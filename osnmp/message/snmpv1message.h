#ifndef SNMPV1MESSAGE_H
#define SNMPV1MESSAGE_H

#include "snmpmessage.h"

namespace osnmp {
    class SnmpV1Message : public SnmpMessage
    {
    public:
        SnmpV1Message();

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

    bool decode(const RawData &data, SnmpV1Message &value);
    bool encode(const SnmpV1Message &value, RawData &data);
}

#endif // SNMPV1MESSAGE_H
