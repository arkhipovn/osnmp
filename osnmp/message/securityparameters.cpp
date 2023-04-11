#include "securityparameters.h"
#include "osnmp/types/all.h"

osnmp::SecurityParameters::SecurityParameters() : authoritativeEngineBoots_(0), authoritativeEngineTime_(0)
{

}

QByteArray osnmp::SecurityParameters::authoritativeEngineID() const
{
    return authoritativeEngineID_;
}

void osnmp::SecurityParameters::setAuthoritativeEngineID(const QByteArray &authoritativeEngineID)
{
    authoritativeEngineID_ = authoritativeEngineID;
}

int osnmp::SecurityParameters::authoritativeEngineBoots() const
{
    return authoritativeEngineBoots_;
}

void osnmp::SecurityParameters::setAuthoritativeEngineBoots(int authoritativeEngineBoots)
{
    authoritativeEngineBoots_ = authoritativeEngineBoots;
}

int osnmp::SecurityParameters::authoritativeEngineTime() const
{
    return authoritativeEngineTime_;
}

void osnmp::SecurityParameters::setAuthoritativeEngineTime(int authoritativeEngineTime)
{
    authoritativeEngineTime_ = authoritativeEngineTime;
}

QByteArray osnmp::SecurityParameters::userName() const
{
    return userName_;
}

void osnmp::SecurityParameters::setUserName(const QByteArray &userName)
{
    userName_ = userName;
}

QByteArray osnmp::SecurityParameters::authenticationParameters() const
{
    return authenticationParameters_;
}

void osnmp::SecurityParameters::setAuthenticationParameters(const QByteArray &authenticationParameters)
{
    authenticationParameters_ = authenticationParameters;
}

QByteArray osnmp::SecurityParameters::privacyParameters() const
{
    return privacyParameters_;
}

void osnmp::SecurityParameters::setPrivacyParameters(const QByteArray &privacyParameters)
{
    privacyParameters_ = privacyParameters;
}

// ______________________

bool osnmp::decode(const RawData &data, SecurityParameters &value)
{
    Q_UNUSED(data)
    Q_UNUSED(value)

    //@N Не знаю почему в SNMP так сделано..
    // Приходит id - OctetString
    // А в этой строке по факту скрыта последовательность..

    OctetString secParamOctetString;
    if(!decode(data, secParamOctetString)) return false;

    if(secParamOctetString.isEmpty()) return false;
    auto msg = RawData::fromByteArray(secParamOctetString);

    auto content = static_cast<ConstructedContent*>(msg.content());
    if(content->size() != 6) return false;

    OctetString authoritativeEngineID;
    if(!decode(content->at(0), authoritativeEngineID)) return false;
    value.setAuthoritativeEngineID(authoritativeEngineID);

    int authoritativeEngineBoots;
    if(!decode(content->at(1), authoritativeEngineBoots)) return false;
    value.setAuthoritativeEngineBoots(authoritativeEngineBoots);

    int authoritativeEngineTime;
    if(!decode(content->at(2), authoritativeEngineTime)) return false;
    value.setAuthoritativeEngineTime(authoritativeEngineTime);

    OctetString userName;
    if(!decode(content->at(3), userName)) return false;
    value.setUserName(userName);

    OctetString authenticationParameters;
    if(!decode(content->at(4), authenticationParameters)) return false;
    value.setAuthenticationParameters(authenticationParameters);

    OctetString privacyParameters;
    if(!decode(content->at(5), privacyParameters)) return false;
    value.setPrivacyParameters(privacyParameters);

    return true;
}

bool osnmp::encode(const SecurityParameters &value, RawData &data)
{
    Sequence sec = {
        value.authoritativeEngineID(),
        value.authoritativeEngineBoots(),
        value.authoritativeEngineTime(),
        value.userName(),
        value.authenticationParameters(),
        value.privacyParameters()
    };

    RawData msg;
    if(!encode(sec, msg)) return false;
    data = std::move(RawData(UniversalTagNumber::OctetString, new PrimitiveContent(msg.toByteArray())));
    return true;
}
