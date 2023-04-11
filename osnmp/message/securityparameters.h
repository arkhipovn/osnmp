#ifndef SECURITYPARAMETERS_H
#define SECURITYPARAMETERS_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    class SecurityParameters
    {
    public:
        SecurityParameters();

        QByteArray authoritativeEngineID() const;
        void setAuthoritativeEngineID(const QByteArray &authoritativeEngineID);

        int authoritativeEngineBoots() const;
        void setAuthoritativeEngineBoots(int authoritativeEngineBoots);

        int authoritativeEngineTime() const;
        void setAuthoritativeEngineTime(int authoritativeEngineTime);

        QByteArray userName() const;
        void setUserName(const QByteArray &userName);

        QByteArray authenticationParameters() const;
        void setAuthenticationParameters(const QByteArray &authenticationParameters);

        QByteArray privacyParameters() const;
        void setPrivacyParameters(const QByteArray &privacyParameters);

    private:
        QByteArray authoritativeEngineID_;
        int authoritativeEngineBoots_;
        int authoritativeEngineTime_;
        QByteArray userName_;
        QByteArray authenticationParameters_;
        QByteArray privacyParameters_;
    };

    bool decode(const RawData &data, SecurityParameters &value);
    bool encode(const SecurityParameters &value, RawData &data);
}

#endif // SECURITYPARAMETERS_H
