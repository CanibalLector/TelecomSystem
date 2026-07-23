#ifndef CLIENTSLISTMODEL_HPP
#define CLIENTSLISTMODEL_HPP

#include <QAbstractListModel>
#include <QVector>
#include <QString>
#include <QStringList>

namespace Telecom::Server {

// Модель отображения клиентов в таблице
class ClientsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum class ActivityState {
        Disconnected = 0, // Отключен (серый)
        Connected = 1,    // Подключен, ожидает (зеленый)
        Sending = 2       // Активно шлет данные (синий)
    };
    Q_ENUM(ActivityState)

    enum ClientRoles {
        IdRole = Qt::UserRole + 1,
        DescriptionRole,
        CheckedRole,
        ActivityStateRole
    };

    explicit ClientsListModel(QObject *parent = nullptr);

    // Обязательно переопределяемые методы
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    // Метод получения отмеченных клиентов
    QStringList getCheckedIds() const;

    // Методы добавления и удаления клиентов
    bool addClient(const QString &id, const QString &description, ActivityState state);
    bool removeClient(const QString &id);

    // Метод для обновления статуса активности
    void setClientActivityState(const QString &id, ActivityState state);

private:
    // Хранит строку таблицы
    struct ClientItem {
        QString id;
        QString description;
        bool checked;
        ActivityState activityState;
    };

    QVector<ClientItem> m_clients;
    int findRowById(const QString &id) const;
};

} // namespace Telecom::Server

#endif // CLIENTSLISTMODEL_HPP
