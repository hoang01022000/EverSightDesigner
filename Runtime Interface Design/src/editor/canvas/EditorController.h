#pragma once

#include <QList>
#include <QString>

namespace eversight {

class EditorController
{
public:
    static QString inspectorMode(bool hasWidgetSelection);
    static QList<int> normalizedSelection(const QList<int>& requestedIds,
                                          const QList<int>& existingIds);
    static int primarySelection(const QList<int>& normalizedIds, int requestedPrimaryId);
};

} // namespace eversight
