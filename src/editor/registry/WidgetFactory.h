#pragma once

#include "WidgetDescriptor.h"

namespace eversight {

class WidgetFactory
{
public:
    static WidgetDescriptor createDescriptor(const QString& typeId,
                                             const QString& displayName,
                                             const QString& iconText,
                                             WidgetCategory category,
                                             qreal defaultWidth,
                                             qreal defaultHeight,
                                             const QString& backgroundColor,
                                             const QString& borderColor,
                                             const QString& fontColor,
                                             const QVariantMap& defaultProperties = {});
    static void completeDescriptor(WidgetDescriptor& descriptor);
};

} // namespace eversight
