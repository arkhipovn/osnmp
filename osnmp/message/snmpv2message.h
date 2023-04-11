#ifndef SNMPV2MESSAGE_H
#define SNMPV2MESSAGE_H

#include "snmpmessage.h"

namespace osnmp {
    class SnmpV2Message : public SnmpMessage
    {
    public:
        SnmpV2Message(int version = SNMP2c);
        virtual ~SnmpV2Message() = default;

        QString community() const;
        void setCommunity(const QString &community);

        Pdu &pdu();
        Pdu pdu() const;
        void setPdu(const Pdu &pdu);

        void decode(const RawData &data, bool *isok = nullptr);
        RawData encode(bool *isok = nullptr) const;

        bool makeResponse(SnmpConf *conf, MibModule *mib) override;

    protected:
        QString community_;  // community
        Pdu pdu_;

    };

    bool decode(const RawData &data, SnmpV2Message &value);
    bool encode(const SnmpV2Message &value, RawData &data);
}

#endif // SNMPV2MESSAGE_H
