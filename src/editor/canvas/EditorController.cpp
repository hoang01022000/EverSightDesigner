#include "EditorController.h"

namespace eversight {

QString EditorController::inspectorMode(bool hasWidgetSelection)
{
    return hasWidgetSelection ? QStringLiteral("widget") : QStringLiteral("container");
}

QList<int> EditorController::normalizedSelection(const QList<int>& requestedIds,
                                                 const QList<int>& existingIds)
{
    QList<int> normalized;
    for (int id : requestedIds) {
        if (existingIds.contains(id) && !normalized.contains(id))
            normalized.append(id);
    }
    return normalized;
}

int EditorController::primarySelection(const QList<int>& normalizedIds, int requestedPrimaryId)
{
    if (normalizedIds.isEmpty())
        return -1;
    return normalizedIds.contains(requestedPrimaryId) ? requestedPrimaryId : normalizedIds.last();
}

} // namespace eversight
