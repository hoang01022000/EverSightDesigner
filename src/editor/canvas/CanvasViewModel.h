#pragma once

#include <QAbstractListModel>
#include "../../shared/models/WidgetItem.h"

class CanvasViewModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedWidgetId READ selectedWidgetId NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedWidgetType READ selectedWidgetType NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedWidgetTitle READ selectedWidgetTitle NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal selectedWidgetX READ selectedWidgetX NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal selectedWidgetY READ selectedWidgetY NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal selectedWidgetWidth READ selectedWidgetWidth NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal selectedWidgetHeight READ selectedWidgetHeight NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedDataSource READ selectedDataSource NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedControlType READ selectedControlType NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedButtonColor READ selectedButtonColor NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedBorderColor READ selectedBorderColor NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedIconColor READ selectedIconColor NOTIFY selectedWidgetChanged)
    Q_PROPERTY(bool selectedAutoFill READ selectedAutoFill NOTIFY selectedWidgetChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY selectedWidgetChanged)
    Q_PROPERTY(int widgetCount READ widgetCount NOTIFY widgetCountChanged)

public:
    explicit CanvasViewModel(QObject* parent = nullptr);

    enum Roles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        XRole,
        YRole,
        WidthRole,
        HeightRole,
        TitleRole,
        DataSourceRole,
        ControlTypeRole,
        ButtonColorRole,
        BorderColorRole,
        IconColorRole,
        AutoFillRole
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addWidget(const QString& type);
    Q_INVOKABLE void addWidgetAt(const QString& type, qreal x, qreal y);
    Q_INVOKABLE void selectWidget(int id);
    Q_INVOKABLE void clearSelection();
    Q_INVOKABLE void removeSelectedWidget();
    Q_INVOKABLE void duplicateSelectedWidget();
    Q_INVOKABLE void moveSelectedForward();
    Q_INVOKABLE void moveSelectedBackward();
    Q_INVOKABLE void alignSelected(const QString& mode, qreal canvasWidth, qreal canvasHeight);
    Q_INVOKABLE void updateWidgetGeometry(int id, qreal x, qreal y, qreal width, qreal height);
    Q_INVOKABLE void updateSelectedTitle(const QString& title);
    Q_INVOKABLE void updateSelectedData(const QString& dataSource, const QString& controlType);
    Q_INVOKABLE void updateSelectedAppearance(const QString& buttonColor, const QString& borderColor, const QString& iconColor);
    Q_INVOKABLE void updateSelectedAutoFill(bool autoFill);
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool saveToFile(const QString& filePath) const;
    Q_INVOKABLE bool loadFromFile(const QString& filePath);

    int selectedWidgetId() const;
    QString selectedWidgetType() const;
    QString selectedWidgetTitle() const;
    qreal selectedWidgetX() const;
    qreal selectedWidgetY() const;
    qreal selectedWidgetWidth() const;
    qreal selectedWidgetHeight() const;
    QString selectedDataSource() const;
    QString selectedControlType() const;
    QString selectedButtonColor() const;
    QString selectedBorderColor() const;
    QString selectedIconColor() const;
    bool selectedAutoFill() const;
    bool hasSelection() const;
    int widgetCount() const;

signals:
    void selectedWidgetChanged();
    void widgetCountChanged();

private:
    QList<WidgetItem> m_widgets;

    int m_nextId = 1;
    int m_selectedWidgetId = -1;

    WidgetItem createWidget(const QString& type) const;

    int indexOfWidget(int id) const;
    const WidgetItem* selectedWidget() const;
};
