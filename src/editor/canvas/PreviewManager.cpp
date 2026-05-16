#include "PreviewManager.h"

PreviewManager::PreviewManager(QObject* parent)
    : QObject(parent)
{
}

bool PreviewManager::previewMode() const
{
    return m_previewMode;
}

void PreviewManager::enterPreview()
{
    setPreviewMode(true);
}

void PreviewManager::exitPreview()
{
    setPreviewMode(false);
}

void PreviewManager::setPreviewMode(bool enabled)
{
    if (m_previewMode == enabled)
        return;

    m_previewMode = enabled;
    emit previewModeChanged();
}
