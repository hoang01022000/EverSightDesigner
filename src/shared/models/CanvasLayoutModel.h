#pragma once

#include <QString>

namespace eversight {

struct CanvasLayoutModel
{
    enum BasicLayout {
        L1 = 1,
        L2,
        L3,
        L4,
        L6,
        L9,
        Custom = 0
    } basicLayout = L1;

    int splitTemplate = 0; // 0..7
    bool showGrid = false;
    QString gridLineColor = "#e0e0e0";
    QString backgroundColor = "#ffffff";
};

} // namespace eversight
