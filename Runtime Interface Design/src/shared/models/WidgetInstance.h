#pragma once

#include <QString>
#include <QVariantMap>
#include <QtGlobal>

namespace eversight {

struct WidgetInstance
{
    WidgetInstance() = default;

    int     id = -1;
    int     parentContainerId = 1;
    QString typeId;
    QString title;

    qreal x = 0.0;
    qreal y = 0.0;
    qreal width = 160.0;
    qreal height = 92.0;

    QString backgroundColor = "#2f2f2f";
    QString borderColor = "#272727";
    QString iconColor = "#f6f6f6";

    QString dataSource = "AllProcess";
    QString controlType = "Both Display";

    bool autoFill = false;

    QVariantMap properties;
};

} // namespace eversight
