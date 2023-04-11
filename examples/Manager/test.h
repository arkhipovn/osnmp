#ifndef TESTS_H
#define TESTS_H

#include <QUdpSocket>
#include <QTimer>
#include <QEventLoop>
#include "osnmp/message/snmpv2message.h"
#include "osnmp/message/snmpv1message.h"

class TestSocket : public QObject {
    Q_OBJECT

public:
    explicit TestSocket(quint16 port = 161) : QObject(), socket(new QUdpSocket), port(port) {
        QObject::connect(socket, &QUdpSocket::readyRead, this, &TestSocket::readPendingDatagram);
    }
    ~TestSocket() { delete socket; }

    void open() {
        socket->bind(0);
    }

    bool write(osnmp::SnmpV2Message &msg) {
        osnmp::RawData d = msg.encode();
        auto data = d.toByteArray();
        return socket->writeDatagram(data, QHostAddress::LocalHost, port) == data.size();
    }

    bool write(osnmp::SnmpV1Message &msg) {
        osnmp::RawData d = msg.encode();
        auto data = d.toByteArray();
        return socket->writeDatagram(data, QHostAddress::LocalHost, port) == data.size();
    }

private slots:
    void readPendingDatagram() {
        qDebug() << "read";
        while(socket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            QHostAddress senderAddress;
            quint16 senderPort;
            socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);


            osnmp::ParseError error;
            auto rawData = osnmp::RawData::fromByteArray(datagram, &error);

            osnmp::SnmpV2Message msg;

            bool isok;
            msg.decode(rawData, &isok);
            int step = 0;
        }
    }

private:
    QUdpSocket *socket;
    quint16 port = 161;
};

#endif // TESTS_H
