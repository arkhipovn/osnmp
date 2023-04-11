#include "snmpconf.h"

osnmp::SnmpConf::SnmpConf()
{

}

QMap<QString, osnmp::CommunitySettings> osnmp::SnmpConf::communities() const
{
    return communities_;
}

QMap<QString, osnmp::CommunitySettings> &osnmp::SnmpConf::communities()
{
    return communities_;
}

void osnmp::SnmpConf::setCommunities(const QMap<QString, osnmp::CommunitySettings> &communities)
{
    communities_ = communities;
}

osnmp::TrapSettings osnmp::SnmpConf::trapSettings() const
{
    return trapSettings_;
}

osnmp::TrapSettings &osnmp::SnmpConf::trapSettings()
{
    return trapSettings_;
}

void osnmp::SnmpConf::setTrapSettings(const TrapSettings &trapSettings)
{
    trapSettings_ = trapSettings;
}
