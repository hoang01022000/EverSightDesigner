#pragma once

#include <QObject>

class PreviewManager : public QObject
{
    Q_OBJECT

public:
    explicit PreviewManager(QObject* parent = nullptr);

    bool previewMode() const;

public slots:
    void enterPreview();
    void exitPreview();

signals:
    void previewModeChanged();

private:
    void setPreviewMode(bool enabled);

    bool m_previewMode = false;
};
