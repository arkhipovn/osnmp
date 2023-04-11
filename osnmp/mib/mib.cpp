#include "mib.h"
#include "osnmp.h"

osnmp::Mib::Mib(const OID &oid, AbstractAgentStrategy *strategy) : oid_(oid), parent_(nullptr), strategy_(strategy)
{

}

OID osnmp::Mib::oid() const
{
    return oid_;
}

OID &osnmp::Mib::oid()
{
    return oid_;
}

osnmp::Mib *osnmp::Mib::parent() const
{
    return parent_;
}

OID osnmp::Mib::absoluteOid() const
{
    if(parent_ == nullptr) return oid_;
    return parent_->absoluteOid() + oid_;
}

bool osnmp::Mib::isNull() const
{
    return oid_.isNull();
}

osnmp::MibSearchResult osnmp::Mib::find(const OID &oid, bool exact) const
{
    return findObject(oid, exact, {});
}

osnmp::MibSearchResult osnmp::Mib::find(const OID &oid, const OIDVector &includes) const
{
    return findObject(oid, false, includes);
}

osnmp::AbstractAgentStrategy *osnmp::Mib::strategy() const
{
    if(strategy_ != nullptr) return strategy_;
    if(parent_ != nullptr) return parent_->strategy();
    return nullptr;
}

void osnmp::Mib::setStrategy(AbstractAgentStrategy *strategy)
{
    if(strategy_ != nullptr) delete strategy_;
    strategy_ = strategy;
}

QSet<osnmp::AbstractAgentStrategy *> osnmp::Mib::getStrategySet() const
{
    if(strategy_ == nullptr) return {};
    return { strategy_ };
}

void osnmp::Mib::setOid(const OID &oid)
{
    oid_ = oid;
}

void osnmp::Mib::setParent(Mib *parent)
{
    parent_ = parent;
}
