#pragma once

#include <QObject>

namespace eversight {

class ZoneTypeClass
{
    Q_GADGET
public:
    enum ZoneType {
        Canvas = 0,
        TopFixed,
        BottomFixed,
        LeftFixed,
        RightFixed
    };
    Q_ENUM(ZoneType)
};

} // namespace eversight
