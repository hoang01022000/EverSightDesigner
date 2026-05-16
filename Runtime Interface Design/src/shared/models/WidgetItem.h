#pragma once

#include "WidgetInstance.h"
#include "WidgetModel.h"

struct WidgetItem
{
    WidgetItem() = default;

    int     id     = -1;
    int     parentRegionId = 1;
    QString type;
    QString title;

    qreal x      = 0;
    qreal y      = 0;
    qreal width  = 160;
    qreal height = 92;

    QString buttonColor = "#2f2f2f";
    QString borderColor = "#272727";
    QString iconColor   = "#f6f6f6";

    QString dataSource  = "AllProcess";
    QString controlType = "Both Display";

    bool autoFill = false;

    QVariantMap properties;
};

namespace eversight {

inline WidgetModel toWidgetModel(const ::WidgetItem &w)
{
    WidgetModel m;
    m.id = QString::number(w.id);
    m.type = w.type;
    m.geometry = QRectF(w.x, w.y, w.width, w.height);
    m.properties = w.properties;
    m.properties.insert("parentRegionId", w.parentRegionId);
    return m;
}

} // namespace eversight
