#pragma once

#include <QString>

namespace eversight {

struct ActionModel
{
    QString id;       // internal id
    QString text;     // display text / tooltip
    QString icon;     // icon source path (relative to qml assets)

    ActionModel() = default;
    ActionModel(const QString &id_, const QString &text_, const QString &icon_ = {})
        : id(id_), text(text_), icon(icon_)
    {}
};

} // namespace eversight
