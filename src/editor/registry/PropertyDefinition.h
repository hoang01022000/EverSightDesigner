#pragma once

#include <QString>
#include <QVariantList>
#include <QVariantMap>

namespace eversight {

class PropertyDefinition
{
public:
    static QVariantMap create(const QString& key,
                              const QString& label,
                              const QString& editor,
                              const QVariantList& options = {});
    static QVariantList forWidget(const QString& typeId);
};

} // namespace eversight
