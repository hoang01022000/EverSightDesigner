#pragma once

#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include "../../shared/enums/WidgetCategory.h"

namespace eversight {

struct WidgetTypeDescriptor
{
    QString        typeId;
    QString        displayName;
    QString        iconText;
    QString        iconSource;
    QString        componentSource;
    QVariantList   propertyDefinitions;
    WidgetCategory category = WidgetCategory::Foundation;

    qreal   defaultWidth  = 160.0;
    qreal   defaultHeight = 92.0;

    QString defaultBackgroundColor = "#2f2f2f";
    QString defaultBorderColor     = "#272727";
    QString defaultFontColor       = "#f6f6f6";

    QVariantMap defaultProperties;
};

} // namespace eversight
