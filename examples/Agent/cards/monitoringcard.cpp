#include "monitoringcard.h"

MonitoringCard::MonitoringCard() : Card(SSU::Monitoring), deviceThresholdsEnabled(false)
{

}

QString MonitoringCard::getName() const
{
    return name;
}

void MonitoringCard::setName(const QString &value)
{
    name = value;
}

QString MonitoringCard::getIp() const
{
    return ip;
}

void MonitoringCard::setIp(const QString &value)
{
    ip = value;
}

QString MonitoringCard::getMask() const
{
    return mask;
}

void MonitoringCard::setMask(const QString &value)
{
    mask = value;
}

quint64 MonitoringCard::getPort() const
{
    return port;
}

void MonitoringCard::setPort(const quint64 &value)
{
    port = value;
}

bool MonitoringCard::getDeviceThresholdsEnabled() const
{
    return deviceThresholdsEnabled;
}

void MonitoringCard::setDeviceThresholdsEnabled(bool value)
{
    deviceThresholdsEnabled = value;
}

MonitoringCard::PowerCard MonitoringCard::getPowerCard1() const
{
    return powerCard1;
}

void MonitoringCard::setPowerCard1(const PowerCard &value)
{
    powerCard1 = value;
}

MonitoringCard::PowerCard MonitoringCard::getPowerCard2() const
{
    return powerCard2;
}

void MonitoringCard::setPowerCard2(const PowerCard &value)
{
    powerCard2 = value;
}
