#pragma once

#include "WidgetModel.h"

#include <QVector>
#include <QSizeF>

namespace eversight {

enum class ZoneType
{
    Canvas,
    TopFixed,
    BottomFixed,
    LeftFixed,
    RightFixed
};

struct ZoneModel
{
    ZoneType type{ZoneType::Canvas};
    bool visible{true};
    QSizeF sizeHint{0.0, 0.0};
    QVector<WidgetModel> widgets;

    ZoneModel() = default;
    explicit ZoneModel(ZoneType t)
        : type(t)
    {}
};

} // namespace eversight
