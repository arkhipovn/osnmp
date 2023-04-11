#ifndef SCOPEDPDU_H
#define SCOPEDPDU_H

#include "pdu.h"

namespace osnmp {
    class ScopedPdu
    {
    public:
        ScopedPdu();

        QByteArray contextEngineID() const;
        void setContextEngineID(const QByteArray &contextEngineID);

        QByteArray contextName() const;
        void setContextName(const QByteArray &contextName);

        Pdu &pdu();
        Pdu pdu() const;
        void setPdu(const Pdu &pdu);

    private:
        QByteArray contextEngineID_;
        QByteArray contextName_;
        Pdu pdu_;
    };

    bool decode(const RawData &data, ScopedPdu &value);
    bool encode(const ScopedPdu &value, RawData &data);
}

#endif // SCOPEDPDU_H
