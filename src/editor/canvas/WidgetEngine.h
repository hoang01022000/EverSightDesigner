#pragma once

#include "../../shared/models/CanvasLayoutModel.h"
#include "../../shared/models/WidgetItem.h"

#include <QRectF>

namespace eversight {

class WidgetEngine
{
public:
    static constexpr qreal DEFAULT_DESIGN_WIDTH = 1280.0;
    static constexpr qreal DEFAULT_DESIGN_HEIGHT = 720.0;
    static constexpr qreal WORKSPACE_TOP_BAR_HEIGHT = 42.0;
    static constexpr qreal WORKSPACE_STATUS_BAR_HEIGHT = 24.0;
    static constexpr qreal MIN_WIDGET_SIZE = 24.0;

    static qreal editableHeight(qreal canvasHeight);
    static qreal containerWidth(const LayoutNode* container, qreal canvasWidth);
    static qreal containerHeight(const LayoutNode* container, qreal canvasHeight);
    static QRectF clampedGeometry(const QRectF& requested, const LayoutNode* container,
                                  qreal canvasWidth, qreal canvasHeight);
    static void clampToContainer(WidgetItem& widget, const LayoutNode* container,
                                 qreal canvasWidth, qreal canvasHeight);
};

} // namespace eversight
