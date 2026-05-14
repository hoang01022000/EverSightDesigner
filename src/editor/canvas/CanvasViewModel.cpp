#include "CanvasViewModel.h"
#include "../../editor/registry/WidgetTypeRegistry.h"
#include "../../shared/models/ScreenModel.h"
#include "../../shared/models/CanvasTabModel.h"
#include "../../shared/models/FixedBarState.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <algorithm>

using namespace eversight;

CanvasViewModel::CanvasViewModel(QObject* parent)
    : QAbstractListModel(parent) {}

// Tab and fixed-bar implementations
void CanvasViewModel::addTab()
{
    CanvasTabModel t;
    t.id = m_tabs.size() + 1;
    t.title = QString("Tab %1").arg(t.id);
    beginResetModel();
    m_tabs.append(t);
    endResetModel();
    emit tabsChanged();
}

void CanvasViewModel::removeTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) return;
    beginResetModel();
    m_tabs.removeAt(index);
    if (m_activeTab >= m_tabs.size()) m_activeTab = qMax(0, m_tabs.size() - 1);
    endResetModel();
    emit tabsChanged();
    emit activeTabChanged();
}

void CanvasViewModel::renameTab(int index, const QString& title)
{
    if (index < 0 || index >= m_tabs.size()) return;
    m_tabs[index].title = title;
    emit tabsChanged();
}

void CanvasViewModel::setActiveTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) return;
    if (m_activeTab == index) return;
    m_activeTab = index;
    emit activeTabChanged();
}

int CanvasViewModel::activeTabIndex() const { return m_activeTab; }
int CanvasViewModel::tabCount() const { return m_tabs.size(); }

void CanvasViewModel::setFixedBar(const QString& bar, bool visible)
{
    const QString b = bar.toLower();
    if (b == "top") m_fixedBars.topVisible = visible;
    else if (b == "bottom") m_fixedBars.bottomVisible = visible;
    else if (b == "left") m_fixedBars.leftVisible = visible;
    else if (b == "right") m_fixedBars.rightVisible = visible;
    emit fixedBarChanged();
}

bool CanvasViewModel::isTopFixedVisible() const { return m_fixedBars.topVisible; }
bool CanvasViewModel::isBottomFixedVisible() const { return m_fixedBars.bottomVisible; }
bool CanvasViewModel::isLeftFixedVisible() const { return m_fixedBars.leftVisible; }
bool CanvasViewModel::isRightFixedVisible() const { return m_fixedBars.rightVisible; }

void CanvasViewModel::setBasicLayout(int basicLayout)
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return;
    m_tabs[m_activeTab].layout.basicLayout = static_cast<eversight::CanvasLayoutModel::BasicLayout>(basicLayout);
    emit layoutChanged();
}

void CanvasViewModel::setSplitTemplate(int tmpl)
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return;
    m_tabs[m_activeTab].layout.splitTemplate = tmpl;
    emit layoutChanged();
}

int CanvasViewModel::currentBasicLayout() const
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return eversight::CanvasLayoutModel::L1;
    return int(m_tabs[m_activeTab].layout.basicLayout);
}

int CanvasViewModel::currentSplitTemplate() const
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return 0;
    return m_tabs[m_activeTab].layout.splitTemplate;
}

bool CanvasViewModel::currentShowGrid() const
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return false;
    return m_tabs[m_activeTab].layout.showGrid;
}

QString CanvasViewModel::currentGridLineColor() const
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return QString("#e0e0e0");
    return m_tabs[m_activeTab].layout.gridLineColor;
}

QString CanvasViewModel::currentBackgroundColor() const
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size()) return QString("#ffffff");
    return m_tabs[m_activeTab].layout.backgroundColor;
}

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
    case TypeRole:        return w.type;
    case XRole:           return w.x;
    case YRole:           return w.y;
    case WidthRole:       return w.width;
    case HeightRole:      return w.height;
    case TitleRole:       return w.title;
    case SelectedRole:    return m_selectedWidgetIds.contains(w.id);
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
        { TypeRole,        "widgetType"        },
        { XRole,           "widgetX"           },
        { YRole,           "widgetY"           },
        { WidthRole,       "widgetWidth"       },
        { HeightRole,      "widgetHeight"      },
        { TitleRole,       "widgetTitle"       },
        { SelectedRole,    "widgetSelected"    },
        { DataSourceRole,  "widgetDataSource"  },
        { ControlTypeRole, "widgetControlType" },
        { ButtonColorRole, "widgetButtonColor" },
        { BorderColorRole, "widgetBorderColor" },
        { IconColorRole,   "widgetIconColor"   },
        { AutoFillRole,    "widgetAutoFill"    }
    };
}

// Factory — uses Registry for defaults
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
    addWidgetAt(type, 160.0 + (m_widgets.size() % 6) * 24,
                      120.0 + (m_widgets.size() % 6) * 24);
}

void CanvasViewModel::addWidgetAt(const QString& type, qreal x, qreal y)
{
    pushUndoState();
    beginInsertRows(QModelIndex(), m_widgets.size(), m_widgets.size());
    WidgetItem item = createWidget(type);
    item.id = m_nextId++;
    item.x  = x;
    item.y  = y;
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

    pushUndoState();
    beginResetModel();
    for (int i = rows.size() - 1; i >= 0; --i)
        m_widgets.removeAt(rows.at(i));
    syncScreenFromWidgets();
    m_selectedWidgetIds.clear();
    m_selectedWidgetId = -1;
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
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
    if (rows.isEmpty() || rows.last() >= m_widgets.size() - 1) return;

    pushUndoState();
    beginResetModel();
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    for (int row : rows) {
        if (row < m_widgets.size() - 1 && !m_selectedWidgetIds.contains(m_widgets.at(row + 1).id))
            m_widgets.move(row, row + 1);
    }
    syncScreenFromWidgets();
    endResetModel();
    emitAllWidgetDataChanged();
}

void CanvasViewModel::moveSelectedBackward()
{
    QList<int> rows = selectedRows();
    if (rows.isEmpty() || rows.first() <= 0) return;

    pushUndoState();
    beginResetModel();
    std::sort(rows.begin(), rows.end());
    for (int row : rows) {
        if (row > 0 && !m_selectedWidgetIds.contains(m_widgets.at(row - 1).id))
            m_widgets.move(row, row - 1);
    }
    syncScreenFromWidgets();
    endResetModel();
    emitAllWidgetDataChanged();
}

void CanvasViewModel::bringSelectedToFront()
{
    const QList<int> rows = selectedRows();
    if (rows.isEmpty() || rows.last() >= m_widgets.size() - 1) return;

    pushUndoState();
    QList<WidgetItem> selected;
    for (int row : rows)
        selected.append(m_widgets.at(row));

    beginResetModel();
    for (int i = rows.size() - 1; i >= 0; --i)
        m_widgets.removeAt(rows.at(i));
    for (const WidgetItem& item : selected)
        m_widgets.append(item);
    syncScreenFromWidgets();
    endResetModel();
    emitAllWidgetDataChanged();
}

void CanvasViewModel::bringSelectedForward()
{
    moveSelectedForward();
}

void CanvasViewModel::sendSelectedToBack()
{
    const QList<int> rows = selectedRows();
    if (rows.isEmpty() || rows.first() <= 0) return;

    pushUndoState();
    QList<WidgetItem> selected;
    for (int row : rows)
        selected.append(m_widgets.at(row));

    beginResetModel();
    for (int i = rows.size() - 1; i >= 0; --i)
        m_widgets.removeAt(rows.at(i));
    for (int i = selected.size() - 1; i >= 0; --i)
        m_widgets.prepend(selected.at(i));
    syncScreenFromWidgets();
    endResetModel();
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
    const QList<int> rows = selectedRows();
    if (rows.isEmpty()) return;

    pushUndoState();
    for (int row : rows) {
        WidgetItem& w = m_widgets[row];
        if      (mode == "left")    w.x = 0;
        else if (mode == "hcenter") w.x = (canvasWidth  - w.width)  / 2.0;
        else if (mode == "right")   w.x = canvasWidth  - w.width;
        else if (mode == "top")     w.y = 0;
        else if (mode == "vcenter") w.y = (canvasHeight - w.height) / 2.0;
        else if (mode == "bottom")  w.y = canvasHeight - w.height;
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

void CanvasViewModel::updateSelectedTitle(const QString& title)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].title == title) return;
    pushUndoState();
    m_widgets[row].title = title;
    emit dataChanged(index(row,0), index(row,0), { TitleRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedData(const QString& dataSource, const QString& controlType)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].dataSource == dataSource && m_widgets[row].controlType == controlType) return;
    pushUndoState();
    m_widgets[row].dataSource  = dataSource;
    m_widgets[row].controlType = controlType;
    emit dataChanged(index(row,0), index(row,0), { DataSourceRole, ControlTypeRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAppearance(const QString& buttonColor, const QString& borderColor, const QString& iconColor)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].buttonColor == buttonColor
            && m_widgets[row].borderColor == borderColor
            && m_widgets[row].iconColor == iconColor)
        return;
    pushUndoState();
    m_widgets[row].buttonColor = buttonColor;
    m_widgets[row].borderColor = borderColor;
    m_widgets[row].iconColor   = iconColor;
    emit dataChanged(index(row,0), index(row,0), { ButtonColorRole, BorderColorRole, IconColorRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAutoFill(bool autoFill)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].autoFill == autoFill) return;
    pushUndoState();
    m_widgets[row].autoFill = autoFill;
    emit dataChanged(index(row,0), index(row,0), { AutoFillRole });
    emit selectedWidgetChanged();
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
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
}

bool CanvasViewModel::saveToFile(const QString& filePath) const
{
    QJsonArray widgets;
    for (const WidgetItem& w : m_widgets) {
        QJsonObject obj;
        obj["id"]          = w.id;
        obj["type"]        = w.type;
        obj["title"]       = w.title;
        obj["x"]           = w.x;
        obj["y"]           = w.y;
        obj["width"]       = w.width;
        obj["height"]      = w.height;
        obj["dataSource"]  = w.dataSource;
        obj["controlType"] = w.controlType;
        obj["buttonColor"] = w.buttonColor;
        obj["borderColor"] = w.borderColor;
        obj["iconColor"]   = w.iconColor;
        obj["autoFill"]    = w.autoFill;
        widgets.append(obj);
    }
    QJsonObject root;
    root["schema"]  = "EverSightDesigner.Layout.v1";
    root["nextId"]  = m_nextId;
    root["widgets"] = widgets;
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool CanvasViewModel::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return false;
    const QJsonObject root    = doc.object();
    const QJsonArray  widgets = root["widgets"].toArray();
    QList<WidgetItem> loaded;
    int maxId = 0;
    for (const QJsonValue& val : widgets) {
        const QJsonObject obj = val.toObject();
        WidgetItem w;
        w.id          = obj["id"].toInt();
        w.type        = obj["type"].toString();
        w.title       = obj["title"].toString(w.type);
        w.x           = obj["x"].toDouble();
        w.y           = obj["y"].toDouble();
        w.width       = obj["width"].toDouble(160);
        w.height      = obj["height"].toDouble(92);
        w.dataSource  = obj["dataSource"].toString("AllProcess");
        w.controlType = obj["controlType"].toString("Both Display");
        w.buttonColor = obj["buttonColor"].toString("#2f2f2f");
        w.borderColor = obj["borderColor"].toString("#272727");
        w.iconColor   = obj["iconColor"].toString("#f6f6f6");
        w.autoFill    = obj["autoFill"].toBool(false);
        if (w.id <= 0 || w.type.isEmpty()) continue;
        maxId = qMax(maxId, w.id);
        loaded.append(w);
    }
    beginResetModel();
    m_widgets          = loaded;
    m_nextId           = qMax(root["nextId"].toInt(maxId + 1), maxId + 1);
    m_selectedWidgetId = -1;
    m_selectedWidgetIds.clear();
    // Rebuild shadow screen model to mirror loaded widgets
    m_screen.canvas.widgets.clear();
    for (const WidgetItem &w : qAsConst(m_widgets))
        m_screen.canvas.widgets.append(eversight::toWidgetModel(w));
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
    m_undoStack.clear();
    m_redoStack.clear();
    emit undoRedoChanged();
    return true;
}

// Property getters
int     CanvasViewModel::selectedWidgetId()     const { return m_selectedWidgetId; }
QString CanvasViewModel::selectedWidgetType()   const { auto* w = selectedWidget(); return w ? w->type        : QString(); }
QString CanvasViewModel::selectedWidgetTitle()  const { auto* w = selectedWidget(); return w ? w->title       : QString(); }
qreal   CanvasViewModel::selectedWidgetX()      const { auto* w = selectedWidget(); return w ? w->x           : 0; }
qreal   CanvasViewModel::selectedWidgetY()      const { auto* w = selectedWidget(); return w ? w->y           : 0; }
qreal   CanvasViewModel::selectedWidgetWidth()  const { auto* w = selectedWidget(); return w ? w->width       : 0; }
qreal   CanvasViewModel::selectedWidgetHeight() const { auto* w = selectedWidget(); return w ? w->height      : 0; }
QString CanvasViewModel::selectedDataSource()   const { auto* w = selectedWidget(); return w ? w->dataSource  : QString(); }
QString CanvasViewModel::selectedControlType()  const { auto* w = selectedWidget(); return w ? w->controlType : QString(); }
QString CanvasViewModel::selectedButtonColor()  const { auto* w = selectedWidget(); return w ? w->buttonColor : QString(); }
QString CanvasViewModel::selectedBorderColor()  const { auto* w = selectedWidget(); return w ? w->borderColor : QString(); }
QString CanvasViewModel::selectedIconColor()    const { auto* w = selectedWidget(); return w ? w->iconColor   : QString(); }
bool    CanvasViewModel::selectedAutoFill()     const { auto* w = selectedWidget(); return w ? w->autoFill    : false; }
bool    CanvasViewModel::hasSelection()         const { return selectedWidget() != nullptr; }
int     CanvasViewModel::selectedCount()        const { return m_selectedWidgetIds.size(); }
int     CanvasViewModel::widgetCount()          const { return m_widgets.size(); }
bool    CanvasViewModel::canUndo()              const { return !m_undoStack.isEmpty(); }
bool    CanvasViewModel::canRedo()              const { return !m_redoStack.isEmpty(); }

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

CanvasViewModel::StateSnapshot CanvasViewModel::snapshot() const
{
    StateSnapshot state;
    state.widgets = m_widgets;
    state.selectedWidgetIds = m_selectedWidgetIds;
    state.primarySelectedWidgetId = m_selectedWidgetId;
    state.nextId = m_nextId;
    return state;
}

void CanvasViewModel::restoreSnapshot(const StateSnapshot& state)
{
    beginResetModel();
    m_widgets = state.widgets;
    m_selectedWidgetIds = state.selectedWidgetIds;
    m_selectedWidgetId = state.primarySelectedWidgetId;
    m_nextId = state.nextId;
    syncScreenFromWidgets();
    endResetModel();

    emit widgetCountChanged();
    emit selectedWidgetChanged();
}

void CanvasViewModel::pushUndoState()
{
    m_undoStack.append(snapshot());
    if (m_undoStack.size() > 100)
        m_undoStack.removeFirst();
    m_redoStack.clear();
    emit undoRedoChanged();
}

void CanvasViewModel::syncScreenFromWidgets()
{
    m_screen.canvas.widgets.clear();
    for (const WidgetItem& w : qAsConst(m_widgets))
        m_screen.canvas.widgets.append(eversight::toWidgetModel(w));
}

void CanvasViewModel::emitAllWidgetDataChanged()
{
    if (m_widgets.isEmpty())
        return;

    emit dataChanged(index(0, 0), index(m_widgets.size() - 1, 0),
                     { XRole, YRole, WidthRole, HeightRole, SelectedRole });
}

void CanvasViewModel::setSelection(const QList<int>& ids, int primaryId)
{
    QList<int> normalized;
    for (int id : ids) {
        if (indexOfWidget(id) >= 0 && !normalized.contains(id))
            normalized.append(id);
    }

    if (primaryId >= 0 && !normalized.contains(primaryId))
        primaryId = normalized.isEmpty() ? -1 : normalized.last();

    if (m_selectedWidgetIds == normalized && m_selectedWidgetId == primaryId)
        return;

    m_selectedWidgetIds = normalized;
    m_selectedWidgetId = normalized.isEmpty() ? -1 : primaryId;
    emitAllWidgetDataChanged();
    emit selectedWidgetChanged();
}
