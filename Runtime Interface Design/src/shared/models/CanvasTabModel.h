#pragma once

#include <QString>
#include "CanvasLayoutModel.h"

namespace eversight {

struct CanvasTabModel
{
    int id = -1;
    QString title = "Tab";
    CanvasLayoutModel layout;
};

} // namespace eversight
