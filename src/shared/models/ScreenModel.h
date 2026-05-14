#pragma once

#include "ZoneModel.h"

#include <QString>
#include <QSizeF>

namespace eversight {

struct ScreenModel
{
    QString id;
    QSizeF designResolution{0.0, 0.0};

    ZoneModel canvas;
    ZoneModel topFixed;
    ZoneModel bottomFixed;
    ZoneModel leftFixed;
    ZoneModel rightFixed;

    ScreenModel()
        : canvas(ZoneType::Canvas)
        , topFixed(ZoneType::TopFixed)
        , bottomFixed(ZoneType::BottomFixed)
        , leftFixed(ZoneType::LeftFixed)
        , rightFixed(ZoneType::RightFixed)
    {}

    ScreenModel(const QString &id_, const QSizeF &designResolution_)
        : id(id_)
        , designResolution(designResolution_)
        , canvas(ZoneType::Canvas)
        , topFixed(ZoneType::TopFixed)
        , bottomFixed(ZoneType::BottomFixed)
        , leftFixed(ZoneType::LeftFixed)
        , rightFixed(ZoneType::RightFixed)
    {}
};

} // namespace eversight
