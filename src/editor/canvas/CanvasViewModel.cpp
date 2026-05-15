#include "CanvasViewModel.h"
#include "../../editor/registry/WidgetTypeRegistry.h"
#include "../../shared/models/ScreenModel.h"
#include "../../shared/models/CanvasTabModel.h"
#include "../../shared/models/FixedBarState.h"
#include "EditorController.h"
#include "LayoutEngine.h"
#include "WidgetEngine.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

#include <algorithm>
#include <functional>

using namespace eversight;

namespace {
constexpr qreal DESIGN_WIDTH = WidgetEngine::DESIGN_WIDTH;
constexpr qreal EDITABLE_HEIGHT = WidgetEngine::EDITABLE_HEIGHT;
constexpr qreal MIN_WIDGET_SIZE = WidgetEngine::MIN_WIDGET_SIZE;
}

static QJsonObject layoutNodeToJson(const LayoutNode& node)
{
    QJsonObject obj;
    obj["id"] = node.id;
    obj["parentId"] = node.parentId;
    obj["row"] = node.row;
    obj["column"] = node.column;
    obj["rowSpan"] = node.rowSpan;
    obj["columnSpan"] = node.columnSpan;
    obj["x"] = node.x;
    obj["y"] = node.y;
    obj["width"] = node.width;
    obj["height"] = node.height;
    obj["assignedWidgetId"] = node.assignedWidgetId;

    QJsonArray children;
    for (const LayoutNode& child : node.children)
        children.append(layoutNodeToJson(child));
    obj["children"] = children;
    return obj;
}

static LayoutNode layoutNodeFromJson(const QJsonObject& obj)
{
    LayoutNode node;
    node.id = obj["id"].toInt(1);
    node.parentId = obj["parentId"].toInt(-1);
    node.row = obj["row"].toInt();
    node.column = obj["column"].toInt();
    node.rowSpan = obj["rowSpan"].toInt(1);
    node.columnSpan = obj["columnSpan"].toInt(1);
    node.x = obj["x"].toDouble();
    node.y = obj["y"].toDouble();
    node.width = obj["width"].toDouble(1.0);
    node.height = obj["height"].toDouble(1.0);
    node.assignedWidgetId = obj["assignedWidgetId"].toInt(-1);

    const QJsonArray children = obj["children"].toArray();
    for (const QJsonValue& childValue : children)
        node.children.append(layoutNodeFromJson(childValue.toObject()));
    return node;
}

static QJsonObject canvasLayoutToJson(const CanvasLayoutModel& layout)
{
    QJsonObject obj;
    obj["basicLayout"] = int(layout.basicLayout);
    obj["splitTemplate"] = layout.splitTemplate;
    obj["customRows"] = layout.customRows;
    obj["customColumns"] = layout.customColumns;
    obj["showGrid"] = layout.showGrid;
    obj["gridLineColor"] = layout.gridLineColor;
    obj["backgroundColor"] = layout.backgroundColor;
    obj["nextNodeId"] = layout.nextNodeId;
    obj["root"] = layoutNodeToJson(layout.root);
    return obj;
}

static CanvasLayoutModel canvasLayoutFromJson(const QJsonObject& obj)
{
    CanvasLayoutModel layout;
    layout.basicLayout = static_cast<CanvasLayoutModel::BasicLayout>(obj["basicLayout"].toInt(CanvasLayoutModel::L1));
    layout.splitTemplate = obj["splitTemplate"].toInt();
    layout.customRows = qBound(1, obj["customRows"].toInt(1), 10);
    layout.customColumns = qBound(1, obj["customColumns"].toInt(1), 10);
    layout.showGrid = obj["showGrid"].toBool(false);
    layout.gridLineColor = obj["gridLineColor"].toString("#e0e0e0");
    layout.backgroundColor = obj["backgroundColor"].toString("#ffffff");
    layout.nextNodeId = qMax(2, obj["nextNodeId"].toInt(2));
    if (obj.contains("root"))
        layout.root = layoutNodeFromJson(obj["root"].toObject());
    layout.selectedCellIds.clear();
    return layout;
}

CanvasViewModel::CanvasViewModel(QObject* parent)
    : QAbstractListModel(parent)
{
    CanvasTabModel tab;
    tab.id = 1;
    tab.title = QStringLiteral("Tab 1");
    m_tabs.append(tab);
}

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
    emit layoutChanged();
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
    CanvasLayoutModel* layout = activeLayout();
    if (!layout) return;

    int rows = 1;
    int columns = 1;
    switch (basicLayout) {
    case CanvasLayoutModel::L2Horizontal: rows = 2; columns = 1; break;
    case CanvasLayoutModel::L2Vertical: rows = 1; columns = 2; break;
    case CanvasLayoutModel::L3Horizontal: rows = 3; columns = 1; break;
    case CanvasLayoutModel::L3Vertical: rows = 1; columns = 3; break;
    case CanvasLayoutModel::L4: rows = 2; columns = 2; break;
    case CanvasLayoutModel::L6Wide: rows = 2; columns = 3; break;
    case CanvasLayoutModel::L6Tall: rows = 3; columns = 2; break;
    case CanvasLayoutModel::L9: rows = 3; columns = 3; break;
    case CanvasLayoutModel::Custom:
        setCustomLayout(layout->customRows, layout->customColumns);
        return;
    case CanvasLayoutModel::L1:
    default:
        rows = 1; columns = 1; break;
    }

    layout->basicLayout = static_cast<CanvasLayoutModel::BasicLayout>(basicLayout);
    layout->splitTemplate = 0;
    layout->selectedCellIds.clear();
    layout->root.children.clear();
    layout->root.assignedWidgetId = -1;
    layout->nextNodeId = 2;
    splitLayoutNode(layout->root, rows, columns);
    emit layoutChanged();
}

void CanvasViewModel::setSplitTemplate(int tmpl)
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout) return;

    setLayoutTemplate(*layout, qBound(1, tmpl, 7));
    if (!layout->root.children.isEmpty())
        layout->selectedCellIds = { layout->root.children.first().id };
    else
        layout->selectedCellIds = { layout->root.id };
    emit layoutChanged();
}

void CanvasViewModel::setCustomLayout(int rows, int columns)
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout) return;

    rows = qBound(1, rows, 10);
    columns = qBound(1, columns, 10);
    layout->basicLayout = CanvasLayoutModel::Custom;
    layout->splitTemplate = 0;
    layout->customRows = rows;
    layout->customColumns = columns;
    layout->selectedCellIds.clear();
    layout->root.children.clear();
    layout->root.assignedWidgetId = -1;
    layout->nextNodeId = 2;
    splitLayoutNode(layout->root, rows, columns);
    emit layoutChanged();
}

void CanvasViewModel::splitSelectedLayoutCells(int rows, int columns)
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout) return;

    rows = qBound(1, rows, 10);
    columns = qBound(1, columns, 10);
    QList<int> selected = layout->selectedCellIds;
    if (selected.size() > 1)
        selected = { selected.last() };
    if (selected.isEmpty())
        return;

    pushUndoState();

    for (int id : qAsConst(selected)) {
        LayoutNode* node = findLayoutNode(id);
        if (!node || !node->isLeaf())
            continue;
        const QList<int> oldRegionIds{ id };
        node->assignedWidgetId = -1;
        splitLayoutNode(*node, rows, columns);
        if (!node->children.isEmpty())
            reassignWidgetsToRegion(oldRegionIds, node->children.first().id);
    }

    layout->selectedCellIds.clear();
    if (const LayoutNode* node = findLayoutNode(selected.last())) {
        if (!node->children.isEmpty())
            layout->selectedCellIds = { node->children.first().id };
    }
    emit layoutChanged();
}

void CanvasViewModel::splitSelectedRegionHorizontal()
{
    splitSelectedLayoutCells(2, 1);
}

void CanvasViewModel::splitSelectedRegionVertical()
{
    splitSelectedLayoutCells(1, 2);
}

// Splits one leaf cell into two children at a proportional ratio.
bool CanvasViewModel::splitCell(int cellId, const QString& orientation, qreal ratio)
{
    const LayoutNode* existingNode = findLayoutNode(cellId);
    if (!existingNode || !existingNode->isLeaf())
        return false;

    pushUndoState();
    LayoutNode* node = findLayoutNode(cellId);
    if (!node || !node->isLeaf())
        return false;

    const QList<int> oldRegionIds{ cellId };
    node->assignedWidgetId = -1;
    if (!splitLayoutNodeAtRatio(*node, orientation, ratio))
        return false;

    reassignWidgetsToRegion(oldRegionIds, node->children.first().id);
    if (CanvasLayoutModel* layout = activeLayout())
        layout->selectedCellIds = { node->children.first().id };
    emit layoutChanged();
    return true;
}

// Finds the leaf under a normalized point and splits it at that exact point.
bool CanvasViewModel::splitCellAt(const QString& orientation, qreal normalizedX, qreal normalizedY)
{
    const int targetCellId = cellAt(normalizedX, normalizedY);
    const LayoutNode* target = findLayoutNode(targetCellId);
    if (!target)
        return false;

    const bool vertical = orientation == QStringLiteral("vertical");
    const qreal ratio = vertical
            ? (normalizedX - target->x) / target->width
            : (normalizedY - target->y) / target->height;
    return splitCell(targetCellId, orientation, ratio);
}

void CanvasViewModel::selectLayoutCell(int cellId, bool additive)
{
    const QString oldMode = inspectorMode();
    CanvasLayoutModel* layout = activeLayout();
    if (!layout || !findLayoutNode(cellId)) return;

    if (!additive)
        layout->selectedCellIds.clear();

    if (layout->selectedCellIds.contains(cellId)) {
        if (additive)
            layout->selectedCellIds.removeAll(cellId);
    } else {
        layout->selectedCellIds.append(cellId);
    }

    clearSelection();
    emit layoutChanged();
    if (oldMode != inspectorMode())
        emit inspectorModeChanged();
}

void CanvasViewModel::clearLayoutCellSelection()
{
    const QString oldMode = inspectorMode();
    CanvasLayoutModel* layout = activeLayout();
    if (!layout || layout->selectedCellIds.isEmpty()) return;

    layout->selectedCellIds.clear();
    emit layoutChanged();
    if (oldMode != inspectorMode())
        emit inspectorModeChanged();
}

void CanvasViewModel::assignSelectedWidgetToLayoutCell(int cellId)
{
    if (m_selectedWidgetId < 0)
        return;

    assignWidgetToLayoutCell(m_selectedWidgetId, cellId);
}

void CanvasViewModel::addWidgetToLayoutCell(const QString& type, int cellId)
{
    addWidgetToLayoutCellAt(type, cellId, 12.0, 12.0);
}

void CanvasViewModel::addWidgetToLayoutCellAt(const QString& type, int cellId, qreal x, qreal y)
{
    const LayoutNode* cell = findLayoutNode(cellId);
    if (!cell || !cell->isLeaf())
        return;

    pushUndoState();
    beginInsertRows(QModelIndex(), m_widgets.size(), m_widgets.size());
    WidgetItem item = createWidget(type);
    item.id = m_nextId++;
    item.parentRegionId = cellId;
    item.x = x;
    item.y = y;
    WidgetEngine::clampToContainer(item, cell);
    m_widgets.append(item);
    m_screen.canvas.widgets.append(eversight::toWidgetModel(item));
    endInsertRows();
    emit widgetCountChanged();

    assignWidgetToLayoutCell(item.id, cellId);
    selectWidget(item.id);
}

void CanvasViewModel::resizeLayoutCells(const QString& firstCellId, const QString& secondCellId,
                                        const QString& orientation, qreal deltaRatio)
{
    LayoutNode* first = findLayoutNode(firstCellId.toInt());
    LayoutNode* second = findLayoutNode(secondCellId.toInt());
    if (!first || !second)
        return;

    constexpr qreal MIN_CELL_RATIO = 0.05;
    constexpr qreal EPSILON = 0.0001;
    const bool vertical = orientation == QStringLiteral("vertical");
    const LayoutNode* firstParent = findLayoutParent(first->id);
    const LayoutNode* secondParent = findLayoutParent(second->id);
    if (!firstParent || firstParent != secondParent)
        return;

    if (vertical) {
        const qreal verticalOverlap = qMin(first->y + first->height, second->y + second->height) - qMax(first->y, second->y);
        if (qAbs((first->x + first->width) - second->x) > EPSILON || verticalOverlap <= EPSILON)
            return;
        const qreal sharedLeft = first->x;
        const qreal sharedRight = second->x + second->width;
        const qreal total = sharedRight - sharedLeft;
        if (total <= MIN_CELL_RATIO * 2.0) return;
        const qreal newFirstWidth = qBound(MIN_CELL_RATIO, first->width + deltaRatio, total - MIN_CELL_RATIO);
        const qreal newSecondWidth = total - newFirstWidth;
        setLayoutNodeRect(*first, first->x, first->y, newFirstWidth, first->height);
        setLayoutNodeRect(*second, first->x + newFirstWidth, second->y, newSecondWidth, second->height);
    } else {
        const qreal horizontalOverlap = qMin(first->x + first->width, second->x + second->width) - qMax(first->x, second->x);
        if (qAbs((first->y + first->height) - second->y) > EPSILON || horizontalOverlap <= EPSILON)
            return;
        const qreal sharedTop = first->y;
        const qreal sharedBottom = second->y + second->height;
        const qreal total = sharedBottom - sharedTop;
        if (total <= MIN_CELL_RATIO * 2.0) return;
        const qreal newFirstHeight = qBound(MIN_CELL_RATIO, first->height + deltaRatio, total - MIN_CELL_RATIO);
        const qreal newSecondHeight = total - newFirstHeight;
        setLayoutNodeRect(*first, first->x, first->y, first->width, newFirstHeight);
        setLayoutNodeRect(*second, second->x, first->y + newFirstHeight, second->width, newSecondHeight);
    }

    clampWidgetsToLayoutBounds();
    emit layoutChanged();
}

void CanvasViewModel::mergeSelectedLayoutCells()
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout || layout->selectedCellIds.isEmpty())
        return;

    QVariantList selected;
    for (int id : qAsConst(layout->selectedCellIds))
        selected.append(id);
    if (mergeCells(selected))
        return;

    const int selectedId = layout->selectedCellIds.first();
    const LayoutNode* parent = findLayoutParent(selectedId);
    if (!parent || parent->children.size() != 2)
        return;

    const int siblingId = parent->children.at(0).id == selectedId
            ? parent->children.at(1).id
            : parent->children.at(0).id;
    mergeLayoutSiblings(selectedId, siblingId);
}

bool CanvasViewModel::mergeSelectedContainersHorizontal()
{
    return mergeSelectedContainersByOrientation(QStringLiteral("horizontal"));
}

bool CanvasViewModel::mergeSelectedContainersVertical()
{
    return mergeSelectedContainersByOrientation(QStringLiteral("vertical"));
}

// Merges selected sibling leaf cells when they form one rectangular region.
bool CanvasViewModel::mergeCells(const QVariantList& cellIds)
{
    if (cellIds.size() < 2)
        return false;

    QList<int> ids;
    for (const QVariant& value : cellIds) {
        const int id = value.toInt();
        if (!ids.contains(id))
            ids.append(id);
    }

    LayoutNode* parent = findLayoutParent(ids.first());
    if (!parent)
        return false;

    qreal left = 1.0;
    qreal top = 1.0;
    qreal right = 0.0;
    qreal bottom = 0.0;
    qreal selectedArea = 0.0;
    QHash<int, QRectF> oldRects;
    for (int id : qAsConst(ids)) {
        const LayoutNode* node = findLayoutNode(id);
        if (!node || !node->isLeaf() || node->parentId != parent->id)
            return false;
        oldRects.insert(id, QRectF(node->x, node->y, node->width, node->height));
        left = qMin(left, node->x);
        top = qMin(top, node->y);
        right = qMax(right, node->x + node->width);
        bottom = qMax(bottom, node->y + node->height);
        selectedArea += node->width * node->height;
    }

    const QRectF targetRect(left, top, right - left, bottom - top);
    if (qAbs(selectedArea - targetRect.width() * targetRect.height()) > 0.0001)
        return false;

    const int parentId = parent->id;
    pushUndoState();
    parent = findLayoutNode(parentId);
    if (!parent)
        return false;

    LayoutNode merged;
    merged.id = nextLayoutNodeId(*activeLayout());
    merged.parentId = parentId;
    merged.x = targetRect.x();
    merged.y = targetRect.y();
    merged.width = targetRect.width();
    merged.height = targetRect.height();

    for (int i = parent->children.size() - 1; i >= 0; --i) {
        if (ids.contains(parent->children.at(i).id))
            parent->children.removeAt(i);
    }
    parent->children.append(merged);

    if (parent->children.size() == 1) {
        LayoutNode only = parent->children.first();
        parent->children.clear();
        parent->assignedWidgetId = only.assignedWidgetId;
    }

    const int targetRegionId = parent->isLeaf() ? parent->id : merged.id;
    if (CanvasLayoutModel* layout = activeLayout())
        layout->selectedCellIds = { targetRegionId };

    for (int i = 0; i < m_widgets.size(); ++i) {
        WidgetItem& widget = m_widgets[i];
        if (!ids.contains(widget.parentRegionId))
            continue;
        const QRectF oldRect = oldRects.value(widget.parentRegionId);
        widget.x += (oldRect.x() - targetRect.x()) * DESIGN_WIDTH;
        widget.y += (oldRect.y() - targetRect.y()) * EDITABLE_HEIGHT;
        widget.parentRegionId = targetRegionId;
        emit dataChanged(index(i, 0), index(i, 0), { ParentRegionIdRole, XRole, YRole });
    }
    clampWidgetsToLayoutBounds();
    emit layoutChanged();
    return true;
}

void CanvasViewModel::mergeLayoutSiblings(int firstCellId, int secondCellId)
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout || firstCellId == secondCellId)
        return;

    LayoutNode* parent = findLayoutParent(firstCellId);
    LayoutNode* secondParent = findLayoutParent(secondCellId);
    if (!parent || parent != secondParent)
        return;

    LayoutNode* first = findLayoutNode(firstCellId);
    LayoutNode* second = findLayoutNode(secondCellId);
    if (!first || !second)
        return;

    const QRectF firstRect(first->x, first->y, first->width, first->height);
    const QRectF secondRect(second->x, second->y, second->width, second->height);
    const QRectF mergedRect = firstRect.united(secondRect);
    const qreal selectedArea = firstRect.width() * firstRect.height() + secondRect.width() * secondRect.height();
    const qreal unionArea = mergedRect.width() * mergedRect.height();
    if (qAbs(selectedArea - unionArea) > 0.0001)
        return;

    pushUndoState();

    QList<int> oldRegionIds;
    QHash<int, QRectF> oldRects;
    std::function<void(const LayoutNode&)> collectLeaves = [&](const LayoutNode& node) {
        if (node.isLeaf()) {
            oldRegionIds.append(node.id);
            oldRects.insert(node.id, QRectF(node.x, node.y, node.width, node.height));
            return;
        }
        for (const LayoutNode& child : node.children)
            collectLeaves(child);
    };
    collectLeaves(*first);
    collectLeaves(*second);

    const bool collapseParent = parent->children.size() == 2
            && qAbs(parent->x - mergedRect.x()) < 0.0001
            && qAbs(parent->y - mergedRect.y()) < 0.0001
            && qAbs(parent->width - mergedRect.width()) < 0.0001
            && qAbs(parent->height - mergedRect.height()) < 0.0001;

    int targetRegionId = parent->id;
    QRectF targetRect(parent->x, parent->y, parent->width, parent->height);

    for (int i = parent->children.size() - 1; i >= 0; --i) {
        const int childId = parent->children.at(i).id;
        if (childId == firstCellId || childId == secondCellId)
            parent->children.removeAt(i);
    }

    if (collapseParent) {
        parent->children.clear();
    } else {
        LayoutNode merged;
        merged.id = nextLayoutNodeId(*layout);
        merged.parentId = parent->id;
        merged.x = mergedRect.x();
        merged.y = mergedRect.y();
        merged.width = mergedRect.width();
        merged.height = mergedRect.height();
        merged.rowSpan = 1;
        merged.columnSpan = 1;
        parent->children.append(merged);
        targetRegionId = merged.id;
        targetRect = mergedRect;
    }

    layout->selectedCellIds = { targetRegionId };

    for (int i = 0; i < m_widgets.size(); ++i) {
        WidgetItem& widget = m_widgets[i];
        if (!oldRegionIds.contains(widget.parentRegionId))
            continue;
        const QRectF oldRect = oldRects.value(widget.parentRegionId);
        widget.x += (oldRect.x() - targetRect.x()) * DESIGN_WIDTH;
        widget.y += (oldRect.y() - targetRect.y()) * EDITABLE_HEIGHT;
        widget.parentRegionId = targetRegionId;
        emit dataChanged(index(i, 0), index(i, 0), { ParentRegionIdRole, XRole, YRole });
    }
    clampWidgetsToLayoutBounds();
    syncScreenFromWidgets();
    emit layoutChanged();
}

// Updates a divider ratio while preserving the two sibling rectangles.
bool CanvasViewModel::resizeDivider(int parentCellId, qreal ratio, qreal minFirstRatio, qreal minSecondRatio)
{
    const LayoutNode* existingParent = findLayoutNode(parentCellId);
    if (!existingParent || existingParent->children.size() != 2)
        return false;

    pushUndoState();
    LayoutNode* parent = findLayoutNode(parentCellId);
    if (!parent || parent->children.size() != 2)
        return false;

    ratio = qBound(minFirstRatio, ratio, 1.0 - minSecondRatio);
    LayoutNode& first = parent->children[0];
    LayoutNode& second = parent->children[1];
    const bool vertical = qAbs((first.x + first.width) - second.x) < 0.0001
            || qAbs((second.x + second.width) - first.x) < 0.0001;

    if (vertical) {
        setLayoutNodeRect(first, parent->x, parent->y, parent->width * ratio, parent->height);
        setLayoutNodeRect(second, parent->x + first.width, parent->y, parent->width * (1.0 - ratio), parent->height);
    } else {
        setLayoutNodeRect(first, parent->x, parent->y, parent->width, parent->height * ratio);
        setLayoutNodeRect(second, parent->x, parent->y + first.height, parent->width, parent->height * (1.0 - ratio));
    }

    clampWidgetsToLayoutBounds();
    emit layoutChanged();
    return true;
}

// Sets a divider ratio to an exact user-entered value.
bool CanvasViewModel::setExactRatio(int parentCellId, qreal ratio)
{
    return resizeDivider(parentCellId, qBound<qreal>(0.01, ratio, 0.99));
}

void CanvasViewModel::unmergeSelectedLayoutCell()
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout || layout->selectedCellIds.isEmpty())
        return;

    setBasicLayout(int(CanvasLayoutModel::L4));
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

int CanvasViewModel::currentCustomRows() const
{
    const CanvasLayoutModel* layout = activeLayout();
    return layout ? layout->customRows : 1;
}

int CanvasViewModel::currentCustomColumns() const
{
    const CanvasLayoutModel* layout = activeLayout();
    return layout ? layout->customColumns : 1;
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
    case ParentRegionIdRole: return w.parentRegionId;
    case TypeRole:        return w.type;
    case XRole:           return w.x;
    case YRole:           return w.y;
    case WidthRole:       return w.width;
    case HeightRole:      return w.height;
    case TitleRole:       return w.title;
    case SelectedRole:    return m_selectedWidgetIds.contains(w.id);
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
    WidgetEngine::clampToContainer(item, findLayoutNode(item.parentRegionId));
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
                                                          findLayoutNode(w.parentRegionId));
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

void CanvasViewModel::updateSelectedTitle(const QString& title)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].title == title) return;
    pushUndoState();
    m_widgets[row].title = title;
    m_widgets[row].properties.insert("text", title);
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
    m_widgets[row].properties.insert("dataSource", dataSource);
    m_widgets[row].properties.insert("controlType", controlType);
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
    m_widgets[row].properties.insert("background", buttonColor);
    m_widgets[row].properties.insert("border", borderColor);
    m_widgets[row].properties.insert("iconColor", iconColor);
    emit dataChanged(index(row,0), index(row,0), { ButtonColorRole, BorderColorRole, IconColorRole });
    emit selectedWidgetChanged();
}

QString CanvasViewModel::selectedAppearanceValue(const QString& key) const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    if (key == QStringLiteral("background") || key == QStringLiteral("buttonColor"))
        return w->buttonColor;
    if (key == QStringLiteral("border") || key == QStringLiteral("borderColor"))
        return w->borderColor;
    if (key == QStringLiteral("font") || key == QStringLiteral("iconColor") || key == QStringLiteral("textColor"))
        return w->iconColor;
    return w->properties.value(key).toString();
}

void CanvasViewModel::updateSelectedAppearanceField(const QString& key, const QString& value)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0 || key.isEmpty())
        return;

    WidgetItem& widget = m_widgets[row];
    QList<int> roles;
    bool changed = false;

    if (key == QStringLiteral("background") || key == QStringLiteral("buttonColor")) {
        if (widget.buttonColor == value)
            return;
        pushUndoState();
        widget.buttonColor = value;
        widget.properties.insert(QStringLiteral("background"), value);
        roles.append(ButtonColorRole);
        changed = true;
    } else if (key == QStringLiteral("border") || key == QStringLiteral("borderColor")) {
        if (widget.borderColor == value)
            return;
        pushUndoState();
        widget.borderColor = value;
        widget.properties.insert(QStringLiteral("border"), value);
        roles.append(BorderColorRole);
        changed = true;
    } else if (key == QStringLiteral("font") || key == QStringLiteral("iconColor") || key == QStringLiteral("textColor")) {
        if (widget.iconColor == value)
            return;
        pushUndoState();
        widget.iconColor = value;
        widget.properties.insert(QStringLiteral("iconColor"), value);
        roles.append(IconColorRole);
        changed = true;
    } else {
        if (widget.properties.value(key).toString() == value)
            return;
        pushUndoState();
        widget.properties.insert(key, value);
        changed = true;
    }

    if (!changed)
        return;

    if (!roles.isEmpty())
        emit dataChanged(index(row, 0), index(row, 0), roles);
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAutoFill(bool autoFill)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].autoFill == autoFill) return;
    pushUndoState();
    m_widgets[row].autoFill = autoFill;
    m_widgets[row].properties.insert("autoFill", autoFill);
    emit dataChanged(index(row,0), index(row,0), { AutoFillRole });
    emit selectedWidgetChanged();
}

QVariant CanvasViewModel::selectedPropertyValue(const QString& key) const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    if (key == "dataSource") return w->dataSource;
    if (key == "controlType") return w->controlType;
    if (key == "iconColor") return w->iconColor;
    if (key == "background" || key == "buttonColor") return w->buttonColor;
    if (key == "border" || key == "borderColor") return w->borderColor;
    if (key == "autoFill") return w->autoFill;
    if (key == "text") return w->title;

    return w->properties.value(key);
}

void CanvasViewModel::updateSelectedProperty(const QString& key, const QVariant& value)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0 || key.isEmpty()) return;

    WidgetItem& w = m_widgets[row];
    if (selectedPropertyValue(key) == value) return;

    pushUndoState();

    if (key == "dataSource") {
        w.dataSource = value.toString();
    } else if (key == "controlType") {
        w.controlType = value.toString();
    } else if (key == "iconColor") {
        w.iconColor = value.toString();
    } else if (key == "background" || key == "buttonColor") {
        w.buttonColor = value.toString();
    } else if (key == "border" || key == "borderColor") {
        w.borderColor = value.toString();
    } else if (key == "autoFill") {
        w.autoFill = value.toBool();
    } else if (key == "text") {
        w.title = value.toString();
    }

    w.properties.insert(key, value);
    syncScreenFromWidgets();

    const QModelIndex mi = index(row, 0);
    emit dataChanged(mi, mi, { TitleRole, DataSourceRole, ControlTypeRole,
                               ButtonColorRole, BorderColorRole, IconColorRole,
                               AutoFillRole });
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
    emit layoutChanged();
}

bool CanvasViewModel::saveToFile(const QString& filePath) const
{
    QJsonArray widgets;
    for (const WidgetItem& w : m_widgets) {
        QJsonObject obj;
        obj["id"]          = w.id;
        obj["parentRegionId"] = w.parentRegionId;
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
        obj["properties"]  = QJsonObject::fromVariantMap(w.properties);
        widgets.append(obj);
    }
    QJsonObject root;
    root["schema"]  = "EverSightDesigner.Layout.v1";
    root["nextId"]  = m_nextId;
    root["widgets"] = widgets;
    if (const CanvasLayoutModel* layout = activeLayout())
        root["layout"] = canvasLayoutToJson(*layout);
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
        w.parentRegionId = obj["parentRegionId"].toInt(defaultLeafRegionId());
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
        w.properties  = obj["properties"].toObject().toVariantMap();
        if (w.id <= 0 || w.type.isEmpty()) continue;
        maxId = qMax(maxId, w.id);
        loaded.append(w);
    }
    beginResetModel();
    m_widgets          = loaded;
    m_nextId           = qMax(root["nextId"].toInt(maxId + 1), maxId + 1);
    m_selectedWidgetId = -1;
    m_selectedWidgetIds.clear();
    if (root.contains("layout")) {
        if (m_tabs.isEmpty()) {
            CanvasTabModel tab;
            tab.id = 1;
            tab.title = QStringLiteral("Tab 1");
            m_tabs.append(tab);
            m_activeTab = 0;
        }
        m_tabs[m_activeTab].layout = canvasLayoutFromJson(root["layout"].toObject());
    }
    // Rebuild shadow screen model to mirror loaded widgets
    m_screen.canvas.widgets.clear();
    for (const WidgetItem &w : qAsConst(m_widgets))
        m_screen.canvas.widgets.append(eversight::toWidgetModel(w));
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
    emit layoutChanged();
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

QVariantList CanvasViewModel::selectedPropertyDefinitions() const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w->type);
    return desc ? desc->propertyDefinitions : QVariantList{};
}

bool CanvasViewModel::selectedWidgetHasDataSource() const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return false;

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w->type);
    return desc ? desc->hasDataSource : false;
}

QVariantList CanvasViewModel::selectedAppearanceFields() const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w->type);
    if (!desc)
        return {};

    QVariantList fields;
    for (const QVariant& value : desc->appearanceFields) {
        QVariantMap field = value.toMap();
        const QString key = field.value(QStringLiteral("key")).toString();
        field.insert(QStringLiteral("value"), selectedAppearanceValue(key));
        fields.append(field);
    }
    return fields;
}

qreal CanvasViewModel::canvasWidth() const
{
    return DESIGN_WIDTH;
}

qreal CanvasViewModel::canvasHeight() const
{
    return EDITABLE_HEIGHT;
}

QVariantList CanvasViewModel::layoutCells() const
{
    QVariantList cells;
    const CanvasLayoutModel* layout = activeLayout();
    if (!layout)
        return cells;

    std::function<void(const LayoutNode&, int)> appendLeaves = [&](const LayoutNode& node, int depth) {
        if (node.isLeaf()) {
            QVariantMap cell;
            cell.insert("id", node.id);
            cell.insert("x", node.x);
            cell.insert("y", node.y);
            cell.insert("width", node.width);
            cell.insert("height", node.height);
            cell.insert("row", node.row);
            cell.insert("column", node.column);
            cell.insert("rowSpan", node.rowSpan);
            cell.insert("columnSpan", node.columnSpan);
            cell.insert("depth", depth);
            cell.insert("assignedWidgetId", node.assignedWidgetId);
            cell.insert("selected", layout->selectedCellIds.contains(node.id));
            cells.append(cell);
            return;
        }

        for (const LayoutNode& child : node.children)
            appendLeaves(child, depth + 1);
    };

    appendLeaves(layout->root, 0);
    return cells;
}

QVariantList CanvasViewModel::layoutResizeHandles() const
{
    QVariantList handles;
    const CanvasLayoutModel* layout = activeLayout();
    if (!layout)
        return handles;

    constexpr qreal EPSILON = 0.0001;

    std::function<void(const LayoutNode&)> appendSiblingHandles = [&](const LayoutNode& parent) {
        for (int i = 0; i < parent.children.size(); ++i) {
            const LayoutNode& a = parent.children.at(i);
            for (int j = i + 1; j < parent.children.size(); ++j) {
                const LayoutNode& b = parent.children.at(j);
                const qreal verticalOverlap = qMin(a.y + a.height, b.y + b.height) - qMax(a.y, b.y);
                if (qAbs((a.x + a.width) - b.x) < EPSILON && verticalOverlap > EPSILON) {
                    QVariantMap handle;
                    handle.insert("parentCellId", parent.id);
                    handle.insert("firstCellId", a.id);
                    handle.insert("secondCellId", b.id);
                    handle.insert("orientation", "vertical");
                    handle.insert("ratio", a.width / parent.width);
                    handle.insert("parentWidth", parent.width);
                    handle.insert("parentHeight", parent.height);
                    handle.insert("x", b.x);
                    handle.insert("y", qMax(a.y, b.y) + verticalOverlap / 2.0);
                    handle.insert("start", qMax(a.y, b.y));
                    handle.insert("length", verticalOverlap);
                    handles.append(handle);
                } else if (qAbs((b.x + b.width) - a.x) < EPSILON && verticalOverlap > EPSILON) {
                    QVariantMap handle;
                    handle.insert("parentCellId", parent.id);
                    handle.insert("firstCellId", b.id);
                    handle.insert("secondCellId", a.id);
                    handle.insert("orientation", "vertical");
                    handle.insert("ratio", b.width / parent.width);
                    handle.insert("parentWidth", parent.width);
                    handle.insert("parentHeight", parent.height);
                    handle.insert("x", a.x);
                    handle.insert("y", qMax(a.y, b.y) + verticalOverlap / 2.0);
                    handle.insert("start", qMax(a.y, b.y));
                    handle.insert("length", verticalOverlap);
                    handles.append(handle);
                }

                const qreal horizontalOverlap = qMin(a.x + a.width, b.x + b.width) - qMax(a.x, b.x);
                if (qAbs((a.y + a.height) - b.y) < EPSILON && horizontalOverlap > EPSILON) {
                    QVariantMap handle;
                    handle.insert("parentCellId", parent.id);
                    handle.insert("firstCellId", a.id);
                    handle.insert("secondCellId", b.id);
                    handle.insert("orientation", "horizontal");
                    handle.insert("ratio", a.height / parent.height);
                    handle.insert("parentWidth", parent.width);
                    handle.insert("parentHeight", parent.height);
                    handle.insert("x", qMax(a.x, b.x) + horizontalOverlap / 2.0);
                    handle.insert("y", b.y);
                    handle.insert("start", qMax(a.x, b.x));
                    handle.insert("length", horizontalOverlap);
                    handles.append(handle);
                } else if (qAbs((b.y + b.height) - a.y) < EPSILON && horizontalOverlap > EPSILON) {
                    QVariantMap handle;
                    handle.insert("parentCellId", parent.id);
                    handle.insert("firstCellId", b.id);
                    handle.insert("secondCellId", a.id);
                    handle.insert("orientation", "horizontal");
                    handle.insert("ratio", b.height / parent.height);
                    handle.insert("parentWidth", parent.width);
                    handle.insert("parentHeight", parent.height);
                    handle.insert("x", qMax(a.x, b.x) + horizontalOverlap / 2.0);
                    handle.insert("y", a.y);
                    handle.insert("start", qMax(a.x, b.x));
                    handle.insert("length", horizontalOverlap);
                    handles.append(handle);
                }
            }
            appendSiblingHandles(a);
        }
    };

    appendSiblingHandles(layout->root);
    return handles;
}

int CanvasViewModel::selectedLayoutCellId() const
{
    const CanvasLayoutModel* layout = activeLayout();
    return layout && !layout->selectedCellIds.isEmpty() ? layout->selectedCellIds.last() : -1;
}

int CanvasViewModel::selectedLayoutCellCount() const
{
    const CanvasLayoutModel* layout = activeLayout();
    return layout ? layout->selectedCellIds.size() : 0;
}

bool CanvasViewModel::hasSelectedLayoutCell() const
{
    return selectedLayoutCellId() >= 0;
}

QString CanvasViewModel::inspectorMode() const
{
    return EditorController::inspectorMode(hasSelection());
}

QString CanvasViewModel::selectedContainerName() const
{
    const int id = selectedLayoutCellId();
    return id >= 0 ? QStringLiteral("Container %1").arg(id) : QStringLiteral("Root Container");
}

qreal CanvasViewModel::selectedContainerWidth() const
{
    const LayoutNode* node = findLayoutNode(selectedLayoutCellId());
    return WidgetEngine::containerWidth(node);
}

qreal CanvasViewModel::selectedContainerHeight() const
{
    const LayoutNode* node = findLayoutNode(selectedLayoutCellId());
    return WidgetEngine::containerHeight(node);
}

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
    state.tabs = m_tabs;
    state.activeTab = m_activeTab;
    return state;
}

void CanvasViewModel::restoreSnapshot(const StateSnapshot& state)
{
    beginResetModel();
    m_widgets = state.widgets;
    m_selectedWidgetIds = state.selectedWidgetIds;
    m_selectedWidgetId = state.primarySelectedWidgetId;
    m_nextId = state.nextId;
    m_tabs = state.tabs;
    m_activeTab = state.activeTab;
    syncScreenFromWidgets();
    endResetModel();

    emit widgetCountChanged();
    emit selectedWidgetChanged();
    emit layoutChanged();
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
                     { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole, SelectedRole });
}

void CanvasViewModel::setSelection(const QList<int>& ids, int primaryId)
{
    const QString oldMode = inspectorMode();
    QList<int> existingIds;
    for (const WidgetItem& widget : qAsConst(m_widgets))
        existingIds.append(widget.id);

    const QList<int> normalized = EditorController::normalizedSelection(ids, existingIds);
    primaryId = EditorController::primarySelection(normalized, primaryId);

    if (m_selectedWidgetIds == normalized && m_selectedWidgetId == primaryId)
        return;

    m_selectedWidgetIds = normalized;
    m_selectedWidgetId = normalized.isEmpty() ? -1 : primaryId;
    if (!normalized.isEmpty()) {
        CanvasLayoutModel* layout = activeLayout();
        if (layout)
            layout->selectedCellIds.clear();
    }
    emitAllWidgetDataChanged();
    emit selectedWidgetChanged();
    emit layoutChanged();
    if (oldMode != inspectorMode())
        emit inspectorModeChanged();
}

CanvasLayoutModel* CanvasViewModel::activeLayout()
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size())
        return nullptr;
    return &m_tabs[m_activeTab].layout;
}

const CanvasLayoutModel* CanvasViewModel::activeLayout() const
{
    if (m_activeTab < 0 || m_activeTab >= m_tabs.size())
        return nullptr;
    return &m_tabs[m_activeTab].layout;
}

LayoutNode* CanvasViewModel::findLayoutNode(int id)
{
    return LayoutEngine::findNode(activeLayout(), id);
}

const LayoutNode* CanvasViewModel::findLayoutNode(int id) const
{
    return LayoutEngine::findNode(activeLayout(), id);
}

LayoutNode* CanvasViewModel::findLayoutParent(int childId)
{
    return LayoutEngine::findParent(activeLayout(), childId);
}

const LayoutNode* CanvasViewModel::findLayoutParent(int childId) const
{
    return LayoutEngine::findParent(activeLayout(), childId);
}

void CanvasViewModel::splitLayoutNode(LayoutNode& node, int rows, int columns)
{
    LayoutEngine::splitGrid(activeLayout(), node, rows, columns);
}

// Splits a leaf node into first/second children using an exact ratio.
bool CanvasViewModel::splitLayoutNodeAtRatio(LayoutNode& node, const QString& orientation, qreal ratio)
{
    return LayoutEngine::splitAtRatio(activeLayout(), node, orientation, ratio);
}

void CanvasViewModel::setLayoutNodeRect(LayoutNode& node, qreal x, qreal y, qreal width, qreal height)
{
    LayoutEngine::setNodeRect(node, x, y, width, height);
}

// Returns the leaf cell containing a normalized canvas point.
int CanvasViewModel::cellAt(qreal normalizedX, qreal normalizedY) const
{
    return LayoutEngine::cellAt(activeLayout(), normalizedX, normalizedY);
}

bool CanvasViewModel::mergeSelectedContainersByOrientation(const QString& orientation)
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout || layout->selectedCellIds.size() != 2)
        return false;

    QList<int> ids = layout->selectedCellIds;
    LayoutNode* parent = findLayoutParent(ids.first());
    if (!parent)
        return false;

    constexpr qreal EPSILON = 0.0001;
    qreal left = 1.0;
    qreal top = 1.0;
    qreal right = 0.0;
    qreal bottom = 0.0;

    for (int id : qAsConst(ids)) {
        const LayoutNode* node = findLayoutNode(id);
        if (!node || !node->isLeaf() || node->parentId != parent->id)
            return false;
        left = qMin(left, node->x);
        top = qMin(top, node->y);
        right = qMax(right, node->x + node->width);
        bottom = qMax(bottom, node->y + node->height);
    }

    const bool horizontalMerge = orientation == QStringLiteral("horizontal");
    const LayoutNode* first = findLayoutNode(ids.first());
    const LayoutNode* second = findLayoutNode(ids.last());
    if (!first || !second)
        return false;

    const bool sideBySide = qAbs((first->x + first->width) - second->x) <= EPSILON
            || qAbs((second->x + second->width) - first->x) <= EPSILON;
    const bool stacked = qAbs((first->y + first->height) - second->y) <= EPSILON
            || qAbs((second->y + second->height) - first->y) <= EPSILON;

    for (int id : qAsConst(ids)) {
        const LayoutNode* node = findLayoutNode(id);
        if (horizontalMerge) {
            if (qAbs(node->y - top) > EPSILON || qAbs((node->y + node->height) - bottom) > EPSILON)
                return false;
        } else {
            if (qAbs(node->x - left) > EPSILON || qAbs((node->x + node->width) - right) > EPSILON)
                return false;
        }
    }
    if ((horizontalMerge && !sideBySide) || (!horizontalMerge && !stacked))
        return false;

    QVariantList selected;
    for (int id : qAsConst(ids))
        selected.append(id);
    return mergeCells(selected);
}

void CanvasViewModel::setLayoutTemplate(CanvasLayoutModel& layout, int tmpl)
{
    LayoutEngine::setTemplate(layout, tmpl);
}

int CanvasViewModel::nextLayoutNodeId(CanvasLayoutModel& layout)
{
    return LayoutEngine::nextNodeId(layout);
}

void CanvasViewModel::assignWidgetToLayoutCell(int widgetId, int cellId)
{
    LayoutNode* cell = findLayoutNode(cellId);
    if (!cell || !cell->isLeaf() || indexOfWidget(widgetId) < 0)
        return;

    CanvasLayoutModel* layout = activeLayout();
    if (!layout)
        return;

    std::function<void(LayoutNode&)> clearExisting = [&](LayoutNode& node) {
        if (node.assignedWidgetId == widgetId)
            node.assignedWidgetId = -1;
        for (LayoutNode& child : node.children)
            clearExisting(child);
    };
    clearExisting(layout->root);

    cell->assignedWidgetId = widgetId;
    const int row = indexOfWidget(widgetId);
    if (row >= 0) {
        WidgetItem& widget = m_widgets[row];
        widget.parentRegionId = cellId;
        WidgetEngine::clampToContainer(widget, cell);
        emit dataChanged(index(row, 0), index(row, 0),
                         { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole });
    }
    emit layoutChanged();
}

void CanvasViewModel::updateWidgetGeometryFromLayoutCell(int widgetId, const LayoutNode& cell)
{
    const int row = indexOfWidget(widgetId);
    if (row < 0)
        return;

    WidgetItem& widget = m_widgets[row];
    WidgetEngine::clampToContainer(widget, &cell);

    if (row >= 0 && row < m_screen.canvas.widgets.size())
        m_screen.canvas.widgets[row].geometry = QRectF(widget.x, widget.y, widget.width, widget.height);

    emit dataChanged(index(row, 0), index(row, 0), { XRole, YRole, WidthRole, HeightRole });
}

void CanvasViewModel::clampWidgetsToLayoutBounds()
{
    for (int i = 0; i < m_widgets.size(); ++i) {
        WidgetItem& widget = m_widgets[i];
        const LayoutNode* region = findLayoutNode(widget.parentRegionId);
        if (!region)
            continue;

        WidgetEngine::clampToContainer(widget, region);
        emit dataChanged(index(i, 0), index(i, 0),
                         { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole });
    }
    syncScreenFromWidgets();
}

int CanvasViewModel::defaultLeafRegionId() const
{
    return LayoutEngine::defaultLeafId(activeLayout());
}

void CanvasViewModel::reassignWidgetsToRegion(const QList<int>& oldRegionIds, int newRegionId)
{
    const LayoutNode* target = findLayoutNode(newRegionId);
    if (!target)
        return;

    for (int i = 0; i < m_widgets.size(); ++i) {
        WidgetItem& widget = m_widgets[i];
        if (!oldRegionIds.contains(widget.parentRegionId))
            continue;

        const LayoutNode* oldRegion = findLayoutNode(widget.parentRegionId);
        if (oldRegion) {
            widget.x += (oldRegion->x - target->x) * DESIGN_WIDTH;
            widget.y += (oldRegion->y - target->y) * EDITABLE_HEIGHT;
        }

        widget.parentRegionId = newRegionId;
        WidgetEngine::clampToContainer(widget, target);
        emit dataChanged(index(i, 0), index(i, 0),
                         { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole });
    }
    syncScreenFromWidgets();
}
