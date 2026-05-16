#pragma once

#include "../../shared/models/CanvasTabModel.h"

namespace eversight {

class LayoutEngine
{
public:
    static LayoutNode* findNode(CanvasLayoutModel* layout, int id);
    static const LayoutNode* findNode(const CanvasLayoutModel* layout, int id);
    static LayoutNode* findParent(CanvasLayoutModel* layout, int childId);
    static const LayoutNode* findParent(const CanvasLayoutModel* layout, int childId);

    static void splitGrid(CanvasLayoutModel* layout, LayoutNode& node, int rows, int columns);
    static bool splitAtRatio(CanvasLayoutModel* layout, LayoutNode& node, const QString& orientation, qreal ratio);
    static void setNodeRect(LayoutNode& node, qreal x, qreal y, qreal width, qreal height);
    static int cellAt(const CanvasLayoutModel* layout, qreal normalizedX, qreal normalizedY);
    static void setTemplate(CanvasLayoutModel& layout, int tmpl);
    static int nextNodeId(CanvasLayoutModel& layout);
    static int defaultLeafId(const CanvasLayoutModel* layout);
};

} // namespace eversight
