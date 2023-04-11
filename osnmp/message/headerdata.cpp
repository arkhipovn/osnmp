#include "headerdata.h"
#include "osnmp/types/all.h"

osnmp::HeaderData::HeaderData() : id_(0), maxSize_(0), securityModel_(0)
{

}

int osnmp::HeaderData::id() const
{
    return id_;
}

void osnmp::HeaderData::setId(int id)
{
    id_ = id;
}

int osnmp::HeaderData::maxSize() const
{
    return maxSize_;
}

void osnmp::HeaderData::setMaxSize(int maxSize)
{
    maxSize_ = maxSize;
}

QByteArray osnmp::HeaderData::flags() const
{
    return flags_;
}

void osnmp::HeaderData::setFlags(quint8 flags)
{
    flags_.clear();
    flags_.append(flags);
}

void osnmp::HeaderData::setFlags(const QByteArray &flags)
{
    flags_ = flags;
}

int osnmp::HeaderData::securityModel() const
{
    return securityModel_;
}

void osnmp::HeaderData::setSecurityModel(int securityModel)
{
    securityModel_ = securityModel;
}

bool osnmp::HeaderData::isPrivacy() const
{
    return flags_.isEmpty() ? false
                            : flags_.at(0) & PrivacyFlag;
}

bool osnmp::HeaderData::isReportable() const
{
    return flags_.isEmpty() ? false
                            : flags_.at(0) & ReportableFlag;
}

bool osnmp::HeaderData::isAuthentication() const
{
    return flags_.isEmpty() ? false
                            : flags_.at(0) & AuthenticationFlag;
}

// ___________________

bool osnmp::decode(const RawData &data, HeaderData &value)
{
    if(data.isEmpty() || data.isPrimitive()) return false;

    if(data.identifier() != UniversalTagNumber::Sequence) return false;
    auto content = static_cast<ConstructedContent*>(data.content());
    if(content->size() != 4) return false;

    int id;
    if(!decode(content->at(0), id)) return false;
    value.setId(id);

    int maxSize;
    if(!decode(content->at(1), maxSize)) return false;
    value.setMaxSize(maxSize);

    OctetString flags;
    if(!decode(content->at(2), flags)) return false;
    value.setFlags(flags);

    int securityModel;
    if(!decode(content->at(3), securityModel)) return false;
    value.setSecurityModel(securityModel);

    return true;
}

bool osnmp::encode(const HeaderData &value, RawData &data)
{
    auto content = new ConstructedContent({
                                              value.id(),
                                              value.maxSize(),
                                              value.flags(),
                                              value.securityModel()
                                          });
    data = std::move(RawData(UniversalTagNumber::Sequence, content));
    return true;
}
