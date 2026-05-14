#pragma once

#include <QString>
#include <QRectF>
#include <QVariantMap>

namespace eversight {

struct WidgetModel
{
    QString id;
    QString type;
    QRectF geometry;
    QVariantMap properties;

    WidgetModel() = default;
    WidgetModel(const QString &id_, const QString &type_, const QRectF &geometry_, const QVariantMap &properties_ = {})
        : id(id_), type(type_), geometry(geometry_), properties(properties_)
    {}
};

} // namespace eversight
