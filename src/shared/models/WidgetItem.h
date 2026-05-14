#pragma once

#include <QString>
#include <QVariantMap>
#include <QtGlobal>
#include "WidgetModel.h"

struct WidgetItem
{
    WidgetItem() = default;

    int     id     = -1;
    QString type;
    QString title;

    qreal x      = 0;
    qreal y      = 0;
    qreal width  = 160;
    qreal height = 92;

    // Common appearance
    QString buttonColor = "#2f2f2f";
    QString borderColor = "#272727";
    QString iconColor   = "#f6f6f6";

    // Data binding
    QString dataSource  = "AllProcess";
    QString controlType = "Both Display";

    bool autoFill = false;

    // Type-specific properties (e.g. rows/columns for Table)
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
    return m;
}

} // namespace eversight
