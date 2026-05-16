#include "LayoutEngine.h"
#include "LayoutManager.h"

namespace eversight {

LayoutNode* LayoutEngine::findNode(CanvasLayoutModel* layout, int id)
{
    return LayoutManager::findLayoutNode(layout, id);
}

const LayoutNode* LayoutEngine::findNode(const CanvasLayoutModel* layout, int id)
{
    return LayoutManager::findLayoutNode(layout, id);
}

LayoutNode* LayoutEngine::findParent(CanvasLayoutModel* layout, int childId)
{
    return LayoutManager::findLayoutParent(layout, childId);
}

const LayoutNode* LayoutEngine::findParent(const CanvasLayoutModel* layout, int childId)
{
    return LayoutManager::findLayoutParent(layout, childId);
}

void LayoutEngine::splitGrid(CanvasLayoutModel* layout, LayoutNode& node, int rows, int columns)
{
    LayoutManager::splitLayoutNode(layout, node, rows, columns);
}

bool LayoutEngine::splitAtRatio(CanvasLayoutModel* layout, LayoutNode& node, const QString& orientation, qreal ratio)
{
    return LayoutManager::splitLayoutNodeAtRatio(layout, node, orientation, ratio);
}

void LayoutEngine::setNodeRect(LayoutNode& node, qreal x, qreal y, qreal width, qreal height)
{
    LayoutManager::setLayoutNodeRect(node, x, y, width, height);
}

int LayoutEngine::cellAt(const CanvasLayoutModel* layout, qreal normalizedX, qreal normalizedY)
{
    return LayoutManager::cellAt(layout, normalizedX, normalizedY);
}

void LayoutEngine::setTemplate(CanvasLayoutModel& layout, int tmpl)
{
    LayoutManager::setLayoutTemplate(layout, tmpl);
}

int LayoutEngine::nextNodeId(CanvasLayoutModel& layout)
{
    return LayoutManager::nextLayoutNodeId(layout);
}

int LayoutEngine::defaultLeafId(const CanvasLayoutModel* layout)
{
    return LayoutManager::defaultLeafRegionId(layout);
}

} // namespace eversight
