#pragma once

#include <QString>
#include <QVariantMap>
#include <QtGlobal>

class WidgetItem
{
public:
    WidgetItem() = default;

    int id = -1;

    QString type;

    qreal x = 0;
    qreal y = 0;

    qreal width = 160;
    qreal height = 100;

    QString title;
    QString dataSource = "AllProcess";
    QString controlType = "Both Display";
    QString buttonColor = "#2f2f2f";
    QString borderColor = "#272727";
    QString iconColor = "#f6f6f6";
    bool autoFill = false;

    QVariantMap properties;
};
