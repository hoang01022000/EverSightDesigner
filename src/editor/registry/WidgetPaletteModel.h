#pragma once

#include <QAbstractListModel>
#include <QVector>
#include "WidgetTypeDescriptor.h"

namespace eversight {

class WidgetPaletteModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    enum Roles {
        TypeIdRole = Qt::UserRole + 1,
        DisplayNameRole,
        IconSourceRole,
        CategoryRole,
        DescriptionRole
    };

    explicit WidgetPaletteModel(QObject* parent = nullptr);

    Q_INVOKABLE int count() const;
    Q_INVOKABLE QVariantMap get(int index) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void countChanged();

private:
    QVector<WidgetTypeDescriptor> m_descriptors;
};

} // namespace eversight
