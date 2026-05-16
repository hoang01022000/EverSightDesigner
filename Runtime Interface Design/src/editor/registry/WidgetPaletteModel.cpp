#include "WidgetPaletteModel.h"
#include "WidgetTypeRegistry.h"

namespace eversight {

WidgetPaletteModel::WidgetPaletteModel(QObject* parent)
    : QAbstractListModel(parent)
{
    const auto& list = WidgetTypeRegistry::instance().allDescriptors();
    m_descriptors.reserve(list.size());
    for (const auto& d : list) {
        m_descriptors.push_back(d);
    }
}

int WidgetPaletteModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return int(m_descriptors.size());
}

QVariant WidgetPaletteModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};
    const int row = index.row();
    if (row < 0 || row >= m_descriptors.size()) return {};
    const auto& d = m_descriptors.at(row);
    switch (role) {
    case TypeIdRole: return d.typeId;
    case DisplayNameRole: return d.displayName;
    case IconSourceRole: return d.iconSource;
    case ComponentSourceRole: return d.componentSource;
    case CategoryRole: return int(d.category);
    case DescriptionRole: return d.defaultProperties.value("description", QString()).toString();
    default: return {};
    }
}

QHash<int, QByteArray> WidgetPaletteModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeIdRole] = "typeId";
    roles[DisplayNameRole] = "displayName";
    roles[IconSourceRole] = "iconSource";
    roles[ComponentSourceRole] = "componentSource";
    roles[CategoryRole] = "category";
    roles[DescriptionRole] = "description";
    return roles;
}

int WidgetPaletteModel::count() const
{
    return int(m_descriptors.size());
}

QVariantMap WidgetPaletteModel::get(int index) const
{
    QVariantMap result;
    if (index < 0 || index >= m_descriptors.size()) return result;
    const auto& d = m_descriptors.at(index);
    result.insert("typeId", d.typeId);
    result.insert("displayName", d.displayName);
    result.insert("iconSource", d.iconSource);
    result.insert("componentSource", d.componentSource);
    result.insert("category", int(d.category));
    result.insert("description", d.defaultProperties.value("description", QString()).toString());
    return result;
}

} // namespace eversight
