#pragma once

#include <QObject>
#include <QString>

class TopBarViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString openIcon READ openIcon CONSTANT)
    Q_PROPERTY(QString saveIcon READ saveIcon CONSTANT)
    Q_PROPERTY(QString templateIcon READ templateIcon CONSTANT)
    Q_PROPERTY(QString saveTemplateIcon READ saveTemplateIcon CONSTANT)

    Q_PROPERTY(QString bringToFrontIcon READ bringToFrontIcon CONSTANT)
    Q_PROPERTY(QString bringForwardIcon READ bringForwardIcon CONSTANT)
    Q_PROPERTY(QString sendToBackIcon READ sendToBackIcon CONSTANT)
    Q_PROPERTY(QString sendBackwardIcon READ sendBackwardIcon CONSTANT)

    Q_PROPERTY(QString undoIcon READ undoIcon CONSTANT)
    Q_PROPERTY(QString redoIcon READ redoIcon CONSTANT)

    Q_PROPERTY(QString deleteIcon READ deleteIcon CONSTANT)
    Q_PROPERTY(QString clearIcon READ clearIcon CONSTANT)

    Q_PROPERTY(QString alignLeftIcon READ alignLeftIcon CONSTANT)
    Q_PROPERTY(QString alignCenterIcon READ alignCenterIcon CONSTANT)
    Q_PROPERTY(QString alignRightIcon READ alignRightIcon CONSTANT)
    Q_PROPERTY(QString alignTopIcon READ alignTopIcon CONSTANT)
    Q_PROPERTY(QString alignMiddleIcon READ alignMiddleIcon CONSTANT)
    Q_PROPERTY(QString alignBottomIcon READ alignBottomIcon CONSTANT)

    Q_PROPERTY(QString distributeHIcon READ distributeHIcon CONSTANT)
    Q_PROPERTY(QString distributeVIcon READ distributeVIcon CONSTANT)

    Q_PROPERTY(QString toggleTopIcon READ toggleTopIcon CONSTANT)
    Q_PROPERTY(QString toggleBottomIcon READ toggleBottomIcon CONSTANT)
    Q_PROPERTY(QString toggleLeftIcon READ toggleLeftIcon CONSTANT)
    Q_PROPERTY(QString toggleRightIcon READ toggleRightIcon CONSTANT)

    Q_PROPERTY(QString previewIcon READ previewIcon CONSTANT)
    Q_PROPERTY(QString exportIcon READ exportIcon CONSTANT)

public:
    explicit TopBarViewModel(QObject* parent = nullptr);

    QString openIcon() const;
    QString saveIcon() const;
    QString templateIcon() const;
    QString saveTemplateIcon() const;

    QString bringToFrontIcon() const;
    QString bringForwardIcon() const;
    QString sendToBackIcon() const;
    QString sendBackwardIcon() const;

    QString undoIcon() const;
    QString redoIcon() const;

    QString deleteIcon() const;
    QString clearIcon() const;

    QString alignLeftIcon() const;
    QString alignCenterIcon() const;
    QString alignRightIcon() const;
    QString alignTopIcon() const;
    QString alignMiddleIcon() const;
    QString alignBottomIcon() const;

    QString distributeHIcon() const;
    QString distributeVIcon() const;

    QString toggleTopIcon() const;
    QString toggleBottomIcon() const;
    QString toggleLeftIcon() const;
    QString toggleRightIcon() const;

    QString previewIcon() const;
    QString exportIcon() const;

private:
    // store as simple QString paths relative to qml
    QString m_placeholder = "assets/icons/Image.png";
};
