#include "trapsettings.h"

osnmp::TrapSettings::TrapSettings() : enable_(true), port_(162), community_("public")
{

}

QVector<osnmp::TrapTarget> osnmp::TrapSettings::trapTargets() const
{
    return trapTargets_;
}

void osnmp::TrapSettings::setTrapTargets(const QVector<TrapTarget> &trapTargets)
{
    trapTargets_ = trapTargets;
}

bool osnmp::TrapSettings::enable() const
{
    return enable_;
}

void osnmp::TrapSettings::setEnable(bool enable)
{
    enable_ = enable;
}

void osnmp::TrapSettings::appendTrapTarget(const TrapTarget &target)
{
    if(trapTargets_.contains(target))
        return;
    trapTargets_.append(target);
}

void osnmp::TrapSettings::eraseTrapTarget(const osnmp::TrapTarget &target)
{
    int index = trapTargets_.indexOf(target);
    if(index == -1)
        return;
    trapTargets_.removeAt(index);
}

void osnmp::TrapSettings::clearTrapTargets()
{
    trapTargets_.clear();
}

QString osnmp::TrapSettings::community() const
{
    return community_;
}

void osnmp::TrapSettings::setCommunity(const QString &community)
{
    community_ = community;
}

quint16 osnmp::TrapSettings::port() const
{
    return port_;
}

void osnmp::TrapSettings::setPort(const quint16 &port)
{
    port_ = port;
}
