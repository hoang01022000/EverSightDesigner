#include "CanvasViewModel.h"
#include "LayoutManager.h"
#include "WidgetEngine.h"

#include <QHash>
#include <QRectF>
#include <QVariantMap>

#include <functional>


#define m_widgets       m_widgetStore.widgets()
#define m_nextId        m_widgetStore.nextIdRef()
#define DESIGN_WIDTH    (this->canvasWidth())
#define EDITABLE_HEIGHT (this->canvasHeight() - WidgetEngine::WORKSPACE_STATUS_BAR_HEIGHT)

using namespace eversight;

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

void CanvasViewModel::clearLayoutToRootContainer()
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout)
        return;

    if (layout->root.children.isEmpty())
        return;

    pushUndoState();
    layout->basicLayout = CanvasLayoutModel::L1;
    layout->splitTemplate = 0;
    layout->customRows = 1;
    layout->customColumns = 1;
    layout->root.children.clear();
    layout->root.assignedWidgetId = -1;
    layout->selectedCellIds = { layout->root.id };
    layout->nextNodeId = 2;

    for (int i = 0; i < m_widgets.size(); ++i) {
        WidgetItem& widget = m_widgets[i];
        widget.parentRegionId = layout->root.id;
        WidgetEngine::clampToContainer(widget, &layout->root, canvasWidth(), canvasHeight());
        emit dataChanged(index(i, 0), index(i, 0),
                         { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole });
    }
    syncScreenFromWidgets();
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
    WidgetEngine::clampToContainer(item, cell, canvasWidth(), canvasHeight());
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

void CanvasViewModel::beginResizeDivider(int parentCellId)
{
    const LayoutNode* existingParent = findLayoutNode(parentCellId);
    if (!existingParent || existingParent->children.size() != 2)
        return;

    pushUndoState();
    m_resizingDividerParentCellId = parentCellId;
}

bool CanvasViewModel::resizeDivider(int parentCellId, qreal ratio, qreal minFirstRatio, qreal minSecondRatio)
{
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

    emit layoutChanged();
    return true;
}

bool CanvasViewModel::setExactRatio(int parentCellId, qreal ratio)
{
    beginResizeDivider(parentCellId);
    const bool resized = resizeDivider(parentCellId, qBound<qreal>(0.01, ratio, 0.99));
    endResizeDivider();
    return resized;
}

void CanvasViewModel::endResizeDivider()
{
    if (m_resizingDividerParentCellId < 0)
        return;

    m_resizingDividerParentCellId = -1;
    clampWidgetsToLayoutBounds();
    emit layoutChanged();
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
            cell.insert("selected", !previewMode() && layout->selectedCellIds.contains(node.id));
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

QString CanvasViewModel::selectedContainerName() const
{
    const int id = selectedLayoutCellId();
    return id >= 0 ? QStringLiteral("Container %1").arg(id) : QStringLiteral("Root Container");
}

qreal CanvasViewModel::selectedContainerWidth() const
{
    const LayoutNode* node = findLayoutNode(selectedLayoutCellId());
    return WidgetEngine::containerWidth(node, canvasWidth());
}

qreal CanvasViewModel::selectedContainerHeight() const
{
    const LayoutNode* node = findLayoutNode(selectedLayoutCellId());
    return WidgetEngine::containerHeight(node, canvasHeight());
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
    return LayoutManager::findLayoutNode(activeLayout(), id);
}

const LayoutNode* CanvasViewModel::findLayoutNode(int id) const
{
    return LayoutManager::findLayoutNode(activeLayout(), id);
}

LayoutNode* CanvasViewModel::findLayoutParent(int childId)
{
    return LayoutManager::findLayoutParent(activeLayout(), childId);
}

const LayoutNode* CanvasViewModel::findLayoutParent(int childId) const
{
    return LayoutManager::findLayoutParent(activeLayout(), childId);
}

void CanvasViewModel::splitLayoutNode(LayoutNode& node, int rows, int columns)
{
    LayoutManager::splitLayoutNode(activeLayout(), node, rows, columns);
}

bool CanvasViewModel::splitLayoutNodeAtRatio(LayoutNode& node, const QString& orientation, qreal ratio)
{
    return LayoutManager::splitLayoutNodeAtRatio(activeLayout(), node, orientation, ratio);
}

void CanvasViewModel::setLayoutNodeRect(LayoutNode& node, qreal x, qreal y, qreal width, qreal height)
{
    LayoutManager::setLayoutNodeRect(node, x, y, width, height);
}

int CanvasViewModel::cellAt(qreal normalizedX, qreal normalizedY) const
{
    return LayoutManager::cellAt(activeLayout(), normalizedX, normalizedY);
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
    LayoutManager::setLayoutTemplate(layout, tmpl);
}

int CanvasViewModel::nextLayoutNodeId(CanvasLayoutModel& layout)
{
    return LayoutManager::nextLayoutNodeId(layout);
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
        WidgetEngine::clampToContainer(widget, cell, canvasWidth(), canvasHeight());
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
    WidgetEngine::clampToContainer(widget, &cell, canvasWidth(), canvasHeight());

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

        WidgetEngine::clampToContainer(widget, region, canvasWidth(), canvasHeight());
        emit dataChanged(index(i, 0), index(i, 0),
                         { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole });
    }
    syncScreenFromWidgets();
}

int CanvasViewModel::defaultLeafRegionId() const
{
    return LayoutManager::defaultLeafRegionId(activeLayout());
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
        WidgetEngine::clampToContainer(widget, target, canvasWidth(), canvasHeight());
        emit dataChanged(index(i, 0), index(i, 0),
                         { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole });
    }
    syncScreenFromWidgets();
}


#undef EDITABLE_HEIGHT
#undef DESIGN_WIDTH
#undef m_nextId
#undef m_widgets
