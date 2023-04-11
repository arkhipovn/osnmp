#ifndef SNMPV3MESSAGE_H
#define SNMPV3MESSAGE_H

#include "snmpmessage.h"
#include "securityparameters.h"
#include "headerdata.h"
#include "scopedpdu.h"

namespace osnmp {
    class SnmpV3Message : public SnmpMessage
    {
    public:
        SnmpV3Message();

        Pdu &pdu();
        Pdu pdu() const;
        void setPdu(const Pdu &pdu);

        ScopedPdu &scopedPdu();
        ScopedPdu scopedPdu() const;
        void setScopedPdu(const ScopedPdu &scopedPdu);

        SecurityParameters &secParameters();
        SecurityParameters secParameters() const;
        void setSecParameters(const SecurityParameters &secParameters);

        HeaderData &headerData();
        HeaderData headerData() const;
        void setHeaderData(const HeaderData &headerData);

        void decode(const RawData &data, bool *isok = nullptr);
        RawData encode(bool *isok = nullptr) const;

        bool makeResponse(SnmpConf *conf, MibModule *mib) override;

    protected:
        HeaderData headerData_;
        SecurityParameters secParameters_;
        ScopedPdu scopedPdu_;
    };

    bool decode(const RawData &data, SnmpV3Message &value);
    bool encode(const SnmpV3Message &value, RawData &data);
}

#endif // SNMPV3MESSAGE_H
