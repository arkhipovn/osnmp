#ifndef ABSTRACTAGENT_H
#define ABSTRACTAGENT_H

#include <QObject>

#include "strategy/abstractagentstrategy.h"
#include "osnmp/mib/mibmodule.h"
#include "osnmp/mib/mibtableobject.h"
#include "osnmp/conf/snmpconf.h"

namespace osnmp {
    class AbstractAgent : public QObject
    {
        Q_OBJECT

    public:
        explicit AbstractAgent(QObject *parent = nullptr);
        virtual ~AbstractAgent();

        SnmpConf *conf() const { return conf_; }
        void setConf(SnmpConf *value);

        MibModule *mib() const { return mib_; }
        void setMib(MibModule *value);

    protected:
        MibModule *mib_;
        SnmpConf *conf_;
    };
}


#endif // ABSTRACTAGENT_H
