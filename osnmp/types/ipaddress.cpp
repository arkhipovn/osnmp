#include "ipaddress.h"
#include <QStringList>

bool osnmp::decode(const RawData &data, QHostAddress &value)
{
    value.clear();
    if(data.identifier() != osnmp::kIpAddressId) return false;
    if(data.isEmpty() || !data.isPrimitive()) return false;

    auto content = static_cast<PrimitiveContent*>(data.content());
    QString host;
    for(int i = 0; i < content->size(); ++i) {
        if(i != 0) host.append(".");
        host.append(QString::number(content->at(i)));
    }
    value = QHostAddress(host);
    return true;
}

bool osnmp::encode(const QHostAddress &value, RawData &data)
{
    QByteArray content;
    auto list = value.toString().split(".");
    for(int i = 0; i < list.size(); ++i)
        content.append(list.at(i).toInt());
    data = std::move(RawData(osnmp::kIpAddressId, new PrimitiveContent(content)));
    return true;
}


