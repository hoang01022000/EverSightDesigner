#pragma once

#include "../../shared/models/CanvasLayoutModel.h"
#include "../../shared/models/WidgetItem.h"

#include <QRectF>

namespace eversight {

class WidgetEngine
{
public:
    static constexpr qreal DESIGN_WIDTH = 1280.0;
    static constexpr qreal DESIGN_HEIGHT = 720.0;
    static constexpr qreal WORKSPACE_TOP_BAR_HEIGHT = 42.0;
    static constexpr qreal WORKSPACE_STATUS_BAR_HEIGHT = 24.0;
    static constexpr qreal EDITABLE_HEIGHT = DESIGN_HEIGHT - WORKSPACE_STATUS_BAR_HEIGHT;
    static constexpr qreal MIN_WIDGET_SIZE = 24.0;

    static qreal containerWidth(const LayoutNode* container);
    static qreal containerHeight(const LayoutNode* container);
    static QRectF clampedGeometry(const QRectF& requested, const LayoutNode* container);
    static void clampToContainer(WidgetItem& widget, const LayoutNode* container);
};

} // namespace eversight
