#include "udpagent.h"
#include "osnmp/message/messagefactory.h"

osnmp::UdpAgent::UdpAgent(QObject *parent) : AbstractAgent(parent), socket(nullptr)
{

}

osnmp::UdpAgent::~UdpAgent()
{
    close();
}

bool osnmp::UdpAgent::open(const QHostAddress &address, quint16 port)
{
    //    if(conf_ == nullptr) return false;
    if(socket != nullptr) return false;
    socket = new QUdpSocket;
    QObject::connect(socket, &QUdpSocket::readyRead, this, &UdpAgent::readPendingDatagram);
    auto isok = socket->bind(address, port);
    if(!isok) close();
    return isok;
}

bool osnmp::UdpAgent::open(quint16 port)
{
    //    if(conf_ == nullptr) return false;
    if(socket != nullptr) return false;
    socket = new QUdpSocket;
    QObject::connect(socket, &QUdpSocket::readyRead, this, &UdpAgent::readPendingDatagram);
    auto isok = socket->bind(port);
    if(!isok) close();
    return isok;
}

void osnmp::UdpAgent::close()
{
    if(socket != nullptr) {
        socket->close();
        delete socket;
        socket = nullptr;
    }
}

bool osnmp::UdpAgent::isOpen() const
{
    return socket != nullptr;
}

void osnmp::UdpAgent::sendTrap(TrapV2 &trap) const
{
    if(conf_ == nullptr) return;
    if(!conf_->trapSettings().enable()) return;
    if(conf_->trapSettings().community().isEmpty()) return;

    auto targets = conf_->trapSettings().trapTargets();
    for(int i = 0; i < targets.size(); ++i) {
        sendMessageResponse(targets.at(i), conf_->trapSettings().port(), &trap);
    }
}

void osnmp::UdpAgent::readPendingDatagram()
{
    using namespace osnmp;
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        ParseError error;
        auto rawData = RawData::fromByteArray(datagram, &error);

        //@N обработать ошибки..
        switch (error) {
            case ParseError::WrongLen:
                break;
            case ParseError::WrongType:
                break;
            case ParseError::WrongLenDecoded:
                break;
            case ParseError::WrongTypeDecoded:
                continue;
            default:    // No error
                break;
        }

#ifdef ASYNC_AGENT_SERVER
        emit readyRead(senderAddress, senderPort, rawData);
#else
        auto msg = MessageFactory::create(rawData);
        if(msg == nullptr) continue;
        bool isok = msg->makeResponse(conf_, mib_);
        if(isok) sendMessageResponse(senderAddress, senderPort, msg);
        delete msg;
#endif
    }
}

bool osnmp::UdpAgent::sendMessageResponse(const QHostAddress &address, quint16 port, osnmp::SnmpMessage *msg) const
{
    if(socket == nullptr) return false;
    if(msg == nullptr) return false;
    if(address.isNull()) return false;

    bool isok;
    auto raw = msg->encode(&isok);

    if(!isok) return false;

    ParseError error;
    QByteArray data = raw.toByteArray(&error);
    return error == ParseError::NoError ? socket->writeDatagram(data, address, port) == data.size()
                                        : false;
}

#ifdef ASYNC_AGENT_SERVER
bool osnmp::UdpAgent::sendResponse(const QHostAddress &address, quint16 port, const RawData &rawData) const
{
    if(socket == nullptr) return false;
    if(address.isNull()) return false;
    ParseError error;
    QByteArray data = rawData.toByteArray(&error);
    return error == ParseError::NoError ? socket->writeDatagram(data, address, port) == data.size()
                                        : false;
}
#endif
