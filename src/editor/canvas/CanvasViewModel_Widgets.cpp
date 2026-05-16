#include "CanvasViewModel.h"
#include "../../editor/registry/WidgetTypeRegistry.h"
#include "WidgetEngine.h"

#include <QHash>
#include <QRectF>

#include <algorithm>
#include <functional>


#define m_widgets       m_widgetStore.widgets()
#define m_nextId        m_widgetStore.nextIdRef()
#define DESIGN_WIDTH    (this->canvasWidth())
#define EDITABLE_HEIGHT (this->canvasHeight() - WidgetEngine::WORKSPACE_STATUS_BAR_HEIGHT)

using namespace eversight;

int CanvasViewModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_widgets.size();
}

QVariant CanvasViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};
    const WidgetItem& w = m_widgets.at(index.row());

    switch (role) {
    case IdRole:          return w.id;
    case ParentRegionIdRole: return w.parentRegionId;
    case TypeRole:        return w.type;
    case XRole:           return w.x;
    case YRole:           return w.y;
    case WidthRole:       return w.width;
    case HeightRole:      return w.height;
    case TitleRole:       return w.title;
    case SelectedRole:    return !previewMode() && m_selectedWidgetIds.contains(w.id);
    case ComponentSourceRole: {
        const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w.type);
        return desc ? desc->componentSource : QStringLiteral("widgets/PlaceholderWidget.qml");
    }
    case DataSourceRole:  return w.dataSource;
    case ControlTypeRole: return w.controlType;
    case ButtonColorRole: return w.buttonColor;
    case BorderColorRole: return w.borderColor;
    case IconColorRole:   return w.iconColor;
    case AutoFillRole:    return w.autoFill;
    }
    return {};
}

QHash<int, QByteArray> CanvasViewModel::roleNames() const
{
    return {
        { IdRole,          "widgetId"          },
        { ParentRegionIdRole, "widgetParentRegionId" },
        { TypeRole,        "widgetType"        },
        { XRole,           "widgetX"           },
        { YRole,           "widgetY"           },
        { WidthRole,       "widgetWidth"       },
        { HeightRole,      "widgetHeight"      },
        { TitleRole,       "widgetTitle"       },
        { SelectedRole,    "widgetSelected"    },
        { ComponentSourceRole, "widgetComponentSource" },
        { DataSourceRole,  "widgetDataSource"  },
        { ControlTypeRole, "widgetControlType" },
        { ButtonColorRole, "widgetButtonColor" },
        { BorderColorRole, "widgetBorderColor" },
        { IconColorRole,   "widgetIconColor"   },
        { AutoFillRole,    "widgetAutoFill"    }
    };
}

WidgetItem CanvasViewModel::createWidget(const QString& type) const
{
    WidgetItem item;
    item.type = type;

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(type);

    if (desc) {
        item.title       = desc->displayName;
        item.width       = desc->defaultWidth;
        item.height      = desc->defaultHeight;
        item.buttonColor = desc->defaultBackgroundColor;
        item.borderColor = desc->defaultBorderColor;
        item.iconColor   = desc->defaultFontColor;
        item.properties  = desc->defaultProperties;

        if (desc->defaultProperties.contains("dataSource"))
            item.dataSource = desc->defaultProperties["dataSource"].toString();
        if (desc->defaultProperties.contains("controlType"))
            item.controlType = desc->defaultProperties["controlType"].toString();
    } else {
        item.title = type.isEmpty() ? "Widget" : type;
    }

    return item;
}

void CanvasViewModel::addWidget(const QString& type)
{
    const int regionId = selectedLayoutCellId() >= 0 ? selectedLayoutCellId() : defaultLeafRegionId();
    const LayoutNode* region = findLayoutNode(regionId);
    const qreal maxX = region ? qMax<qreal>(0.0, region->width * DESIGN_WIDTH - 180.0) : 1024.0;
    const qreal maxY = region ? qMax<qreal>(0.0, region->height * EDITABLE_HEIGHT - 120.0) : 560.0;
    addWidgetAt(type,
                qMin(160.0 + (m_widgets.size() % 6) * 24, maxX),
                qMin(120.0 + (m_widgets.size() % 6) * 24, maxY));
}

void CanvasViewModel::addWidgetAt(const QString& type, qreal x, qreal y)
{
    pushUndoState();
    beginInsertRows(QModelIndex(), m_widgets.size(), m_widgets.size());
    WidgetItem item = createWidget(type);
    item.id = m_nextId++;
    item.parentRegionId = selectedLayoutCellId() >= 0 ? selectedLayoutCellId() : defaultLeafRegionId();
    item.x  = x;
    item.y  = y;
    WidgetEngine::clampToContainer(item, findLayoutNode(item.parentRegionId), canvasWidth(), canvasHeight());
    m_widgets.append(item);
    // Keep shadow screen model in sync (canvas zone)
    m_screen.canvas.widgets.append(eversight::toWidgetModel(item));
    endInsertRows();
    emit widgetCountChanged();
    selectWidget(item.id);
}

void CanvasViewModel::selectWidget(int id, bool additive)
{
    if (id < 0) {
        clearSelection();
        return;
    }

    if (indexOfWidget(id) < 0)
        return;

    QList<int> selected = additive ? m_selectedWidgetIds : QList<int>{};
    if (additive && selected.contains(id)) {
        selected.removeAll(id);
        const int primaryId = selected.isEmpty() ? -1 : selected.last();
        setSelection(selected, primaryId);
        return;
    }

    if (!selected.contains(id))
        selected.append(id);

    setSelection(selected, id);
}

void CanvasViewModel::clearSelection()
{
    setSelection({}, -1);
}

void CanvasViewModel::removeSelectedWidget()
{
    const QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;
    const QList<int> removedWidgetIds = m_selectedWidgetIds;

    pushUndoState();
    beginResetModel();
    for (int i = rows.size() - 1; i >= 0; --i)
        m_widgets.removeAt(rows.at(i));
    if (CanvasLayoutModel* layout = activeLayout()) {
        std::function<void(LayoutNode&)> clearRemovedAssignments = [&](LayoutNode& node) {
            if (removedWidgetIds.contains(node.assignedWidgetId))
                node.assignedWidgetId = -1;
            for (LayoutNode& child : node.children)
                clearRemovedAssignments(child);
        };
        clearRemovedAssignments(layout->root);
    }
    syncScreenFromWidgets();
    m_selectedWidgetIds.clear();
    m_selectedWidgetId = -1;
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
    emit canvasSizeChanged();
    emit layoutChanged();
}

void CanvasViewModel::duplicateSelectedWidget()
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    pushUndoState();
    WidgetItem item = m_widgets.at(row);
    item.id     = m_nextId++;
    item.title += " Copy";
    item.x     += 24;
    item.y     += 24;
    beginInsertRows(QModelIndex(), m_widgets.size(), m_widgets.size());
    m_widgets.append(item);
    // Mirror duplicate into shadow screen model
    m_screen.canvas.widgets.append(eversight::toWidgetModel(item));
    endInsertRows();
    emit widgetCountChanged();
    selectWidget(item.id);
}

void CanvasViewModel::moveSelectedForward()
{
    QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;

    pushUndoState();
    beginResetModel();
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    bool changed = false;
    for (int row : rows) {
        if (row < 0 || row >= m_widgets.size())
            continue;
        const int parentRegionId = m_widgets.at(row).parentRegionId;
        int nextRow = -1;
        for (int i = row + 1; i < m_widgets.size(); ++i) {
            if (m_widgets.at(i).parentRegionId != parentRegionId)
                continue;
            if (m_selectedWidgetIds.contains(m_widgets.at(i).id))
                continue;
            nextRow = i;
            break;
        }
        if (nextRow >= 0) {
            m_widgets.move(row, nextRow);
            changed = true;
        }
    }
    syncScreenFromWidgets();
    endResetModel();
    if (changed)
        emitAllWidgetDataChanged();
}

void CanvasViewModel::moveSelectedBackward()
{
    QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;

    pushUndoState();
    beginResetModel();
    std::sort(rows.begin(), rows.end());
    bool changed = false;
    for (int row : rows) {
        if (row < 0 || row >= m_widgets.size())
            continue;
        const int parentRegionId = m_widgets.at(row).parentRegionId;
        int previousRow = -1;
        for (int i = row - 1; i >= 0; --i) {
            if (m_widgets.at(i).parentRegionId != parentRegionId)
                continue;
            if (m_selectedWidgetIds.contains(m_widgets.at(i).id))
                continue;
            previousRow = i;
            break;
        }
        if (previousRow >= 0) {
            m_widgets.move(row, previousRow);
            changed = true;
        }
    }
    syncScreenFromWidgets();
    endResetModel();
    if (changed)
        emitAllWidgetDataChanged();
}

void CanvasViewModel::bringSelectedToFront()
{
    const QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;

    pushUndoState();
    beginResetModel();
    bool changed = false;
    bool moved = true;
    while (moved) {
        moved = false;
        QList<int> currentRows = selectedRows();
        std::sort(currentRows.begin(), currentRows.end(), std::greater<int>());
        for (int row : currentRows) {
            if (row < 0 || row >= m_widgets.size())
                continue;
            const int parentRegionId = m_widgets.at(row).parentRegionId;
            int nextRow = -1;
            for (int i = row + 1; i < m_widgets.size(); ++i) {
                if (m_widgets.at(i).parentRegionId != parentRegionId)
                    continue;
                if (m_selectedWidgetIds.contains(m_widgets.at(i).id))
                    continue;
                nextRow = i;
                break;
            }
            if (nextRow >= 0) {
                m_widgets.move(row, nextRow);
                moved = true;
                changed = true;
            }
        }
    }
    syncScreenFromWidgets();
    endResetModel();
    if (changed)
        emitAllWidgetDataChanged();
}

void CanvasViewModel::bringSelectedForward()
{
    moveSelectedForward();
}

void CanvasViewModel::sendSelectedToBack()
{
    const QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;

    pushUndoState();
    beginResetModel();
    bool changed = false;
    bool moved = true;
    while (moved) {
        moved = false;
        QList<int> currentRows = selectedRows();
        std::sort(currentRows.begin(), currentRows.end());
        for (int row : currentRows) {
            if (row < 0 || row >= m_widgets.size())
                continue;
            const int parentRegionId = m_widgets.at(row).parentRegionId;
            int previousRow = -1;
            for (int i = row - 1; i >= 0; --i) {
                if (m_widgets.at(i).parentRegionId != parentRegionId)
                    continue;
                if (m_selectedWidgetIds.contains(m_widgets.at(i).id))
                    continue;
                previousRow = i;
                break;
            }
            if (previousRow >= 0) {
                m_widgets.move(row, previousRow);
                moved = true;
                changed = true;
            }
        }
    }
    syncScreenFromWidgets();
    endResetModel();
    if (changed)
        emitAllWidgetDataChanged();
}

void CanvasViewModel::sendSelectedBackward()
{
    moveSelectedBackward();
}

void CanvasViewModel::distributeSelectedHorizontal()
{
    QList<int> rows = selectedRows();
    if (rows.size() < 3) return;

    std::sort(rows.begin(), rows.end(), [this](int a, int b) {
        return m_widgets.at(a).x < m_widgets.at(b).x;
    });

    qreal left = m_widgets.at(rows.first()).x;
    qreal right = m_widgets.at(rows.first()).x + m_widgets.at(rows.first()).width;
    qreal totalWidth = 0;
    for (int row : rows) {
        const WidgetItem& w = m_widgets.at(row);
        left = qMin(left, w.x);
        right = qMax(right, w.x + w.width);
        totalWidth += w.width;
    }

    const qreal spacing = (right - left - totalWidth) / qreal(rows.size() - 1);
    pushUndoState();
    qreal x = left;
    for (int row : rows) {
        m_widgets[row].x = x;
        x += m_widgets[row].width + spacing;
    }
    syncScreenFromWidgets();
    emitAllWidgetDataChanged();
    emit selectedWidgetChanged();
}

void CanvasViewModel::distributeSelectedVertical()
{
    QList<int> rows = selectedRows();
    if (rows.size() < 3) return;

    std::sort(rows.begin(), rows.end(), [this](int a, int b) {
        return m_widgets.at(a).y < m_widgets.at(b).y;
    });

    qreal top = m_widgets.at(rows.first()).y;
    qreal bottom = m_widgets.at(rows.first()).y + m_widgets.at(rows.first()).height;
    qreal totalHeight = 0;
    for (int row : rows) {
        const WidgetItem& w = m_widgets.at(row);
        top = qMin(top, w.y);
        bottom = qMax(bottom, w.y + w.height);
        totalHeight += w.height;
    }

    const qreal spacing = (bottom - top - totalHeight) / qreal(rows.size() - 1);
    pushUndoState();
    qreal y = top;
    for (int row : rows) {
        m_widgets[row].y = y;
        y += m_widgets[row].height + spacing;
    }
    syncScreenFromWidgets();
    emitAllWidgetDataChanged();
    emit selectedWidgetChanged();
}

void CanvasViewModel::undo()
{
    if (m_undoStack.isEmpty()) return;
    m_redoStack.append(snapshot());
    const StateSnapshot state = m_undoStack.takeLast();
    restoreSnapshot(state);
    emit undoRedoChanged();
}

void CanvasViewModel::redo()
{
    if (m_redoStack.isEmpty()) return;
    m_undoStack.append(snapshot());
    const StateSnapshot state = m_redoStack.takeLast();
    restoreSnapshot(state);
    emit undoRedoChanged();
}

void CanvasViewModel::fitToWindow()
{
    // Stub: fit-to-window handled by view layer or later implementation
}

int CanvasViewModel::fittedZoomPercent(qreal viewportWidth, qreal viewportHeight,
                                       qreal designWidth, qreal designHeight,
                                       qreal horizontalPadding, qreal verticalPadding) const
{
    if (viewportWidth <= 0 || viewportHeight <= 0 || designWidth <= 0 || designHeight <= 0)
        return 100;

    const qreal availableWidth = qMax<qreal>(1.0, viewportWidth - horizontalPadding);
    const qreal availableHeight = qMax<qreal>(1.0, viewportHeight - verticalPadding);
    const qreal fit = qMin(availableWidth / designWidth, availableHeight / designHeight);
    return qBound(40, int(fit * 100.0), 160);
}

void CanvasViewModel::alignSelected(const QString& mode, qreal canvasWidth, qreal canvasHeight)
{
    Q_UNUSED(canvasWidth)
    Q_UNUSED(canvasHeight)

    const QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;

    pushUndoState();
    for (int row : rows) {
        WidgetItem& w = m_widgets[row];
        const LayoutNode* region = findLayoutNode(w.parentRegionId);
        const qreal regionWidth = region ? region->width * DESIGN_WIDTH : DESIGN_WIDTH;
        const qreal regionHeight = region ? region->height * EDITABLE_HEIGHT : EDITABLE_HEIGHT;
        if      (mode == "left")    w.x = 0;
        else if (mode == "hcenter") w.x = (regionWidth  - w.width)  / 2.0;
        else if (mode == "right")   w.x = regionWidth  - w.width;
        else if (mode == "top")     w.y = 0;
        else if (mode == "vcenter") w.y = (regionHeight - w.height) / 2.0;
        else if (mode == "bottom")  w.y = regionHeight - w.height;
        w.x = qBound<qreal>(0.0, w.x, qMax<qreal>(0.0, regionWidth - w.width));
        w.y = qBound<qreal>(0.0, w.y, qMax<qreal>(0.0, regionHeight - w.height));
    }

    syncScreenFromWidgets();
    emitAllWidgetDataChanged();
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateWidgetGeometry(int id, qreal x, qreal y, qreal width, qreal height)
{
    const int row = indexOfWidget(id);
    if (row < 0) return;
    WidgetItem& w = m_widgets[row];
    const QRectF geometry = WidgetEngine::clampedGeometry(QRectF(x, y, width, height),
                                                          findLayoutNode(w.parentRegionId),
                                                          canvasWidth(), canvasHeight());
    x = geometry.x();
    y = geometry.y();
    width = geometry.width();
    height = geometry.height();
    if (qFuzzyCompare(w.x, x) && qFuzzyCompare(w.y, y)
            && qFuzzyCompare(w.width, width) && qFuzzyCompare(w.height, height))
        return;

    pushUndoState();
    w.x = x; w.y = y;
    w.width  = qMax<qreal>(24, width);
    w.height = qMax<qreal>(24, height);
    if (row >= 0 && row < m_screen.canvas.widgets.size()) {
        m_screen.canvas.widgets[row].geometry = QRectF(w.x, w.y, w.width, w.height);
    }
    const QModelIndex mi = index(row, 0);
    emit dataChanged(mi, mi, { XRole, YRole, WidthRole, HeightRole });
    if (m_selectedWidgetId == id) emit selectedWidgetChanged();
}

void CanvasViewModel::clear()
{
    if (m_widgets.isEmpty()) return;
    pushUndoState();
    beginResetModel();
    m_widgets.clear();
    // Keep shadow screen model empty as well
    m_screen.canvas.widgets.clear();
    m_selectedWidgetId = -1;
    m_selectedWidgetIds.clear();
    if (CanvasLayoutModel* layout = activeLayout()) {
        std::function<void(LayoutNode&)> clearAssignments = [&](LayoutNode& node) {
            node.assignedWidgetId = -1;
            for (LayoutNode& child : node.children)
                clearAssignments(child);
        };
        clearAssignments(layout->root);
    }
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
    emit canvasSizeChanged();
    emit layoutChanged();
}

int     CanvasViewModel::widgetCount()          const { return m_widgets.size(); }

int CanvasViewModel::indexOfWidget(int id) const
{
    for (int i = 0; i < m_widgets.size(); ++i)
        if (m_widgets.at(i).id == id) return i;
    return -1;
}

const WidgetItem* CanvasViewModel::selectedWidget() const
{
    const int row = indexOfWidget(m_selectedWidgetId);
    return row >= 0 ? &m_widgets.at(row) : nullptr;
}

QList<int> CanvasViewModel::selectedRows() const
{
    QList<int> rows;
    for (int i = 0; i < m_widgets.size(); ++i) {
        if (m_selectedWidgetIds.contains(m_widgets.at(i).id))
            rows.append(i);
    }
    return rows;
}


#undef EDITABLE_HEIGHT
#undef DESIGN_WIDTH
#undef m_nextId
#undef m_widgets
