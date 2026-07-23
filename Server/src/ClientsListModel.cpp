#include "ClientsListModel.hpp"
#include <QDebug>

namespace Telecom::Server {

ClientsListModel::ClientsListModel(QObject *parent) : QAbstractListModel(parent) {
}

int ClientsListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_clients.count();
}

QVariant ClientsListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_clients.count()) return QVariant();

    const auto &item = m_clients[index.row()];

    switch (role) {
    case IdRole:            return item.id;
    case DescriptionRole:   return item.description;
    case CheckedRole:       return item.checked;
    case ActivityStateRole: return static_cast<int>(item.activityState);
    default:                return QVariant();
    }
}

bool ClientsListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_clients.count()) return false;

    if (role == CheckedRole) {
        m_clients[index.row()].checked = value.toBool();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QHash<int, QByteArray> ClientsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole]            = "clientId";
    roles[DescriptionRole]  = "clientDescription";
    roles[CheckedRole]       = "clientChecked";
    roles[ActivityStateRole] = "clientActivityState";
    return roles;
}

int ClientsListModel::findRowById(const QString &id) const {
    for (int i = 0; i < m_clients.count(); ++i) {
        if (m_clients[i].id == id) return i;
    }
    return -1;
}

QStringList ClientsListModel::getCheckedIds() const {
    QStringList checkedIds;
    for (const auto &item : m_clients) {
        if (item.checked) {
            checkedIds.append(item.id);
        }
    }
    return checkedIds;
}

bool ClientsListModel::addClient(const QString &id, const QString &description, ActivityState state) {
    if (id.isEmpty()) {
        return false;
    }

    // Если клиент уже есть, просто обновляем ему статус
    if( findRowById(id) != -1 ) {
        setClientActivityState(id, state);
        return true;
    }

    int insertRow = m_clients.count();
    beginInsertRows(QModelIndex(), insertRow, insertRow);

    m_clients.append({ id, description, false, state });

    endInsertRows();
    return true;
}

bool ClientsListModel::removeClient(const QString &id) {
    int removeRow = findRowById(id);
    if (removeRow == -1) {
        return false;
    }

    beginRemoveRows(QModelIndex(), removeRow, removeRow);
    m_clients.removeAt(removeRow);
    endRemoveRows();
    return true;
}


void ClientsListModel::setClientActivityState(const QString &id, ActivityState state) {
    int rowIndex = findRowById(id);
    if (rowIndex == -1) return;

    if (m_clients[rowIndex].activityState == state) return;

    m_clients[rowIndex].activityState = state;
    QModelIndex modelIndex = createIndex(rowIndex, 0);

    emit dataChanged(modelIndex, modelIndex, {ActivityStateRole});
}

} // namespace Telecom::Server
