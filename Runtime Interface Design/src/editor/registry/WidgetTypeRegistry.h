#pragma once

#include <QHash>
#include <QList>
#include <QString>
#include "WidgetTypeDescriptor.h"
#include "../../shared/enums/WidgetCategory.h"

namespace eversight {

class WidgetTypeRegistry
{
public:
    static const WidgetTypeRegistry& instance();

    const WidgetTypeDescriptor* descriptor(const QString& typeId) const;
    const QList<WidgetTypeDescriptor>& allDescriptors() const;
    QList<const WidgetTypeDescriptor*> descriptorsForCategory(WidgetCategory category) const;

private:
    WidgetTypeRegistry();

    QList<WidgetTypeDescriptor> m_descriptors;
    QHash<QString, int>         m_indexByTypeId;
};

} // namespace eversight
