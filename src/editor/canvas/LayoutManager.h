#pragma once

#include "../../shared/models/CanvasTabModel.h"

namespace eversight {

class LayoutManager
{
public:
    static eversight::LayoutNode* findLayoutNode(eversight::CanvasLayoutModel* layout, int id);
    static const eversight::LayoutNode* findLayoutNode(const eversight::CanvasLayoutModel* layout, int id);
    static eversight::LayoutNode* findLayoutParent(eversight::CanvasLayoutModel* layout, int childId);
    static const eversight::LayoutNode* findLayoutParent(const eversight::CanvasLayoutModel* layout, int childId);

    static void splitLayoutNode(eversight::CanvasLayoutModel* layout, eversight::LayoutNode& node, int rows, int columns);
    static bool splitLayoutNodeAtRatio(eversight::CanvasLayoutModel* layout, eversight::LayoutNode& node, const QString& orientation, qreal ratio);
    static void setLayoutNodeRect(eversight::LayoutNode& node, qreal x, qreal y, qreal width, qreal height);
    static int cellAt(const eversight::CanvasLayoutModel* layout, qreal normalizedX, qreal normalizedY);
    static void setLayoutTemplate(eversight::CanvasLayoutModel& layout, int tmpl);
    static int nextLayoutNodeId(eversight::CanvasLayoutModel& layout);
    static int defaultLeafRegionId(const eversight::CanvasLayoutModel* layout);
};

} // namespace eversight
