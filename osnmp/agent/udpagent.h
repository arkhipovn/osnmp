#ifndef UDPAGENT_H
#define UDPAGENT_H

#include <QUdpSocket>
#include "abstractagent.h"
#include "osnmp/message/trapv2.h"

namespace osnmp {
    class SnmpMessage;
    class UdpAgent : public AbstractAgent
    {
        Q_OBJECT

    public:
        explicit UdpAgent(QObject *parent = nullptr);
        ~UdpAgent();

        bool open(const QHostAddress &address, quint16 port = 161);
        bool open(quint16 port = 161);
        void close();

        bool isOpen() const;

    public slots:
        // Отправка автоматического сообщения всем зарегистрированным пользователям
        void sendTrap(TrapV2 &trap) const;

#ifdef ASYNC_AGENT_SERVER
        /*
            Ответ пользователю на запрос
            То есть когда сформировали ответ SnmpMessage
            Нужно закодировать данные
            bool isok;
            auto raw = msg->encode(&isok);
        */
        bool sendResponse(const QHostAddress &address, quint16 port, const RawData &rawData) const;

    signals:
        // Получен запрос от пользователя
        void readyRead(const QHostAddress &address, quint16 port, const RawData &rawData);
#endif

    private slots:
        void readPendingDatagram();
        bool sendMessageResponse(const QHostAddress &address, quint16 port, SnmpMessage *msg) const;

    private:
        QUdpSocket *socket;
    };
}

Q_DECLARE_METATYPE(QHostAddress)

#endif // UDPAGENT_H
