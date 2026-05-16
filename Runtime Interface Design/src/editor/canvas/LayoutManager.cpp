#include "LayoutManager.h"

#include <QRectF>
#include <functional>

namespace eversight {

eversight::LayoutNode* LayoutManager::findLayoutNode(eversight::CanvasLayoutModel* layout, int id)
{
    if (!layout) return nullptr;
    std::function<eversight::LayoutNode*(eversight::LayoutNode&)> find = [&](eversight::LayoutNode& node) -> eversight::LayoutNode* {
        if (node.id == id) return &node;
        for (eversight::LayoutNode& child : node.children) {
            if (eversight::LayoutNode* found = find(child)) return found;
        }
        return nullptr;
    };
    return find(layout->root);
}

const eversight::LayoutNode* LayoutManager::findLayoutNode(const eversight::CanvasLayoutModel* layout, int id)
{
    if (!layout) return nullptr;
    std::function<const eversight::LayoutNode*(const eversight::LayoutNode&)> find = [&](const eversight::LayoutNode& node) -> const eversight::LayoutNode* {
        if (node.id == id) return &node;
        for (const eversight::LayoutNode& child : node.children) {
            if (const eversight::LayoutNode* found = find(child)) return found;
        }
        return nullptr;
    };
    return find(layout->root);
}

eversight::LayoutNode* LayoutManager::findLayoutParent(eversight::CanvasLayoutModel* layout, int childId)
{
    if (!layout) return nullptr;
    std::function<eversight::LayoutNode*(eversight::LayoutNode&)> find = [&](eversight::LayoutNode& node) -> eversight::LayoutNode* {
        for (eversight::LayoutNode& child : node.children) {
            if (child.id == childId) return &node;
            if (eversight::LayoutNode* found = find(child)) return found;
        }
        return nullptr;
    };
    return find(layout->root);
}

const eversight::LayoutNode* LayoutManager::findLayoutParent(const eversight::CanvasLayoutModel* layout, int childId)
{
    if (!layout) return nullptr;
    std::function<const eversight::LayoutNode*(const eversight::LayoutNode&)> find = [&](const eversight::LayoutNode& node) -> const eversight::LayoutNode* {
        for (const eversight::LayoutNode& child : node.children) {
            if (child.id == childId) return &node;
            if (const eversight::LayoutNode* found = find(child)) return found;
        }
        return nullptr;
    };
    return find(layout->root);
}

void LayoutManager::splitLayoutNode(eversight::CanvasLayoutModel* layout, eversight::LayoutNode& node, int rows, int columns)
{
    rows = qBound(1, rows, 10);
    columns = qBound(1, columns, 10);
    node.children.clear();
    if (rows == 1 && columns == 1) return;
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            eversight::LayoutNode child;
            child.id = layout ? nextLayoutNodeId(*layout) : (row * columns + column + 2);
            child.parentId = node.id;
            child.row = row;
            child.column = column;
            child.rowSpan = 1;
            child.columnSpan = 1;
            child.x = node.x + node.width * qreal(column) / qreal(columns);
            child.y = node.y + node.height * qreal(row) / qreal(rows);
            child.width = node.width / qreal(columns);
            child.height = node.height / qreal(rows);
            node.children.append(child);
        }
    }
}

bool LayoutManager::splitLayoutNodeAtRatio(eversight::CanvasLayoutModel* layout, eversight::LayoutNode& node, const QString& orientation, qreal ratio)
{
    if (!layout || !node.isLeaf()) return false;
    ratio = qBound<qreal>(0.01, ratio, 0.99);
    const bool vertical = orientation == QStringLiteral("vertical");
    node.children.clear();

    eversight::LayoutNode first;
    first.id = nextLayoutNodeId(*layout);
    first.parentId = node.id;
    first.x = node.x;
    first.y = node.y;
    first.width = vertical ? node.width * ratio : node.width;
    first.height = vertical ? node.height : node.height * ratio;

    eversight::LayoutNode second;
    second.id = nextLayoutNodeId(*layout);
    second.parentId = node.id;
    second.x = vertical ? node.x + first.width : node.x;
    second.y = vertical ? node.y : node.y + first.height;
    second.width = vertical ? node.width * (1.0 - ratio) : node.width;
    second.height = vertical ? node.height : node.height * (1.0 - ratio);

    node.children.append(first);
    node.children.append(second);
    return true;
}

void LayoutManager::setLayoutNodeRect(eversight::LayoutNode& node, qreal x, qreal y, qreal width, qreal height)
{
    const QRectF oldRect(node.x, node.y, node.width, node.height);
    const QRectF newRect(x, y, width, height);

    node.x = newRect.x();
    node.y = newRect.y();
    node.width = newRect.width();
    node.height = newRect.height();

    if (node.children.isEmpty() || oldRect.width() <= 0.0 || oldRect.height() <= 0.0)
        return;

    for (eversight::LayoutNode& child : node.children) {
        const qreal relativeX = (child.x - oldRect.x()) / oldRect.width();
        const qreal relativeY = (child.y - oldRect.y()) / oldRect.height();
        const qreal relativeWidth = child.width / oldRect.width();
        const qreal relativeHeight = child.height / oldRect.height();
        setLayoutNodeRect(child,
                          newRect.x() + relativeX * newRect.width(),
                          newRect.y() + relativeY * newRect.height(),
                          relativeWidth * newRect.width(),
                          relativeHeight * newRect.height());
    }
}

int LayoutManager::cellAt(const eversight::CanvasLayoutModel* layout, qreal normalizedX, qreal normalizedY)
{
    if (!layout) return -1;
    std::function<int(const eversight::LayoutNode&)> find = [&](const eversight::LayoutNode& node) -> int {
        if (normalizedX < node.x || normalizedX > node.x + node.width
                || normalizedY < node.y || normalizedY > node.y + node.height)
            return -1;
        if (node.isLeaf()) return node.id;
        for (const eversight::LayoutNode& child : node.children) {
            const int id = find(child);
            if (id >= 0) return id;
        }
        return -1;
    };
    return find(layout->root);
}

void LayoutManager::setLayoutTemplate(eversight::CanvasLayoutModel& layout, int tmpl)
{
    layout.basicLayout = eversight::CanvasLayoutModel::Custom;
    layout.splitTemplate = tmpl;
    layout.selectedCellIds.clear();
    layout.root.children.clear();
    layout.root.assignedWidgetId = -1;
    layout.nextNodeId = 2;

    auto addCell = [&](int row, int column, int rowSpan, int columnSpan, int gridRows, int gridColumns) {
        eversight::LayoutNode child;
        child.id = nextLayoutNodeId(layout);
        child.parentId = layout.root.id;
        child.row = row;
        child.column = column;
        child.rowSpan = rowSpan;
        child.columnSpan = columnSpan;
        child.x = qreal(column) / qreal(gridColumns);
        child.y = qreal(row) / qreal(gridRows);
        child.width = qreal(columnSpan) / qreal(gridColumns);
        child.height = qreal(rowSpan) / qreal(gridRows);
        layout.root.children.append(child);
    };

    switch (tmpl) {
    case 2:
        addCell(0, 0, 1, 2, 2, 3);
        addCell(1, 0, 1, 1, 2, 3);
        addCell(1, 1, 1, 1, 2, 3);
        addCell(0, 2, 2, 1, 2, 3);
        break;
    case 3:
        addCell(0, 0, 1, 3, 3, 3);
        addCell(1, 0, 2, 1, 3, 3);
        addCell(1, 1, 1, 1, 3, 3);
        addCell(1, 2, 1, 1, 3, 3);
        addCell(2, 1, 1, 2, 3, 3);
        break;
    case 4:
        addCell(0, 0, 2, 2, 3, 3);
        addCell(0, 2, 1, 1, 3, 3);
        addCell(1, 2, 1, 1, 3, 3);
        addCell(2, 0, 1, 1, 3, 3);
        addCell(2, 1, 1, 2, 3, 3);
        break;
    case 5:
        addCell(0, 0, 1, 1, 3, 3);
        addCell(0, 1, 1, 2, 3, 3);
        addCell(1, 0, 1, 2, 3, 3);
        addCell(1, 2, 2, 1, 3, 3);
        addCell(2, 0, 1, 1, 3, 3);
        addCell(2, 1, 1, 1, 3, 3);
        break;
    case 6:
        addCell(0, 0, 3, 1, 3, 4);
        addCell(0, 1, 1, 2, 3, 4);
        addCell(0, 3, 1, 1, 3, 4);
        addCell(1, 1, 2, 1, 3, 4);
        addCell(1, 2, 1, 2, 3, 4);
        addCell(2, 2, 1, 2, 3, 4);
        break;
    case 7:
        addCell(0, 0, 1, 1, 4, 4);
        addCell(0, 1, 1, 2, 4, 4);
        addCell(0, 3, 2, 1, 4, 4);
        addCell(1, 0, 2, 2, 4, 4);
        addCell(1, 2, 1, 1, 4, 4);
        addCell(2, 2, 2, 2, 4, 4);
        addCell(3, 0, 1, 2, 4, 4);
        break;
    case 1:
    default:
        addCell(0, 0, 2, 2, 2, 3);
        addCell(0, 2, 1, 1, 2, 3);
        addCell(1, 2, 1, 1, 2, 3);
        break;
    }
}

int LayoutManager::nextLayoutNodeId(eversight::CanvasLayoutModel& layout)
{
    return layout.nextNodeId++;
}

int LayoutManager::defaultLeafRegionId(const eversight::CanvasLayoutModel* layout)
{
    if (!layout) return 1;
    std::function<int(const eversight::LayoutNode&)> findLeaf = [&](const eversight::LayoutNode& node) -> int {
        if (node.isLeaf()) return node.id;
        for (const eversight::LayoutNode& child : node.children) {
            const int id = findLeaf(child);
            if (id >= 0) return id;
        }
        return -1;
    };
    const int id = findLeaf(layout->root);
    return id >= 0 ? id : layout->root.id;
}

} // namespace eversight
