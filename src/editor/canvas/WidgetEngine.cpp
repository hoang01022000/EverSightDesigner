#include "WidgetEngine.h"

#include <QtGlobal>

namespace eversight {

qreal WidgetEngine::containerWidth(const LayoutNode* container)
{
    return container ? container->width * DESIGN_WIDTH : DESIGN_WIDTH;
}

qreal WidgetEngine::containerHeight(const LayoutNode* container)
{
    return container ? container->height * EDITABLE_HEIGHT : EDITABLE_HEIGHT;
}

QRectF WidgetEngine::clampedGeometry(const QRectF& requested, const LayoutNode* container)
{
    const qreal boundsWidth = containerWidth(container);
    const qreal boundsHeight = containerHeight(container);
    const qreal width = qBound(MIN_WIDGET_SIZE, requested.width(), qMax(MIN_WIDGET_SIZE, boundsWidth));
    const qreal height = qBound(MIN_WIDGET_SIZE, requested.height(), qMax(MIN_WIDGET_SIZE, boundsHeight));
    const qreal x = qBound<qreal>(0.0, requested.x(), qMax<qreal>(0.0, boundsWidth - width));
    const qreal y = qBound<qreal>(0.0, requested.y(), qMax<qreal>(0.0, boundsHeight - height));
    return QRectF(x, y, width, height);
}

void WidgetEngine::clampToContainer(WidgetItem& widget, const LayoutNode* container)
{
    const QRectF rect = clampedGeometry(QRectF(widget.x, widget.y, widget.width, widget.height), container);
    widget.x = rect.x();
    widget.y = rect.y();
    widget.width = rect.width();
    widget.height = rect.height();
}

} // namespace eversight
