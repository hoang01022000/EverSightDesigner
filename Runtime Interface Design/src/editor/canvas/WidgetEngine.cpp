#include "WidgetEngine.h"

#include <QtGlobal>

namespace eversight {

qreal WidgetEngine::editableHeight(qreal canvasHeight)
{
    return qMax<qreal>(MIN_WIDGET_SIZE, canvasHeight - WORKSPACE_STATUS_BAR_HEIGHT);
}

qreal WidgetEngine::containerWidth(const LayoutNode* container, qreal canvasWidth)
{
    return container ? container->width * canvasWidth : canvasWidth;
}

qreal WidgetEngine::containerHeight(const LayoutNode* container, qreal canvasHeight)
{
    const qreal height = editableHeight(canvasHeight);
    return container ? container->height * height : height;
}

QRectF WidgetEngine::clampedGeometry(const QRectF& requested, const LayoutNode* container,
                                     qreal canvasWidth, qreal canvasHeight)
{
    const qreal boundsWidth = containerWidth(container, canvasWidth);
    const qreal boundsHeight = containerHeight(container, canvasHeight);
    const qreal width = qBound(MIN_WIDGET_SIZE, requested.width(), qMax(MIN_WIDGET_SIZE, boundsWidth));
    const qreal height = qBound(MIN_WIDGET_SIZE, requested.height(), qMax(MIN_WIDGET_SIZE, boundsHeight));
    const qreal x = qBound<qreal>(0.0, requested.x(), qMax<qreal>(0.0, boundsWidth - width));
    const qreal y = qBound<qreal>(0.0, requested.y(), qMax<qreal>(0.0, boundsHeight - height));
    return QRectF(x, y, width, height);
}

void WidgetEngine::clampToContainer(WidgetItem& widget, const LayoutNode* container,
                                    qreal canvasWidth, qreal canvasHeight)
{
    const QRectF rect = clampedGeometry(QRectF(widget.x, widget.y, widget.width, widget.height),
                                        container, canvasWidth, canvasHeight);
    widget.x = rect.x();
    widget.y = rect.y();
    widget.width = rect.width();
    widget.height = rect.height();
}

} // namespace eversight
