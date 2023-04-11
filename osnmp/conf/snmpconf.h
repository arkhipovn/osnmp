#ifndef SNMPCONF_H
#define SNMPCONF_H

#include <QMap>
#include "communitysettings.h"
#include "trapsettings.h"

namespace osnmp {
    struct SnmpConf
    {
    public:
        SnmpConf();

        TrapSettings trapSettings() const;
        TrapSettings &trapSettings();
        void setTrapSettings(const TrapSettings &trapSettings);

        QMap<QString, osnmp::CommunitySettings> communities() const;
        QMap<QString, osnmp::CommunitySettings> &communities();
        void setCommunities(const QMap<QString, osnmp::CommunitySettings> &communities);

    private:
        // Для v1/v2
        QMap <QString, osnmp::CommunitySettings> communities_;
        TrapSettings trapSettings_;
    };
}

#endif // SNMPCONF_H
