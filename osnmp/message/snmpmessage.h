#ifndef SNMPMESSAGE_H
#define SNMPMESSAGE_H

#include "osnmp/message/pdu.h"
#include "osnmp/conf/snmpconf.h"
#include "osnmp/mib/mibmodule.h"

namespace osnmp {
    class SnmpMessage
    {
    public:
        SnmpMessage() : version_(0) {}
        SnmpMessage(int version) : version_(version) {}
        virtual ~SnmpMessage() = default;
        //        virtual ~SnmpMessage() { qDebug() << "~SnmpMessage"; }

        void setVersion(int value) { version_ = value; }
        int version() const { return version_; }

        virtual Pdu &pdu() = 0;
        virtual Pdu pdu() const = 0;
        virtual void setPdu(const Pdu &pdu) = 0;

        virtual void decode(const RawData &data, bool *isok = nullptr) = 0;
        virtual RawData encode(bool *isok = nullptr) const = 0;

        // Готовит ответ для текущего сообщения
        virtual bool makeResponse(SnmpConf *conf, MibModule *mib) = 0;

    protected:
        int version_;
    };
}

#endif // SNMPMESSAGE_H

