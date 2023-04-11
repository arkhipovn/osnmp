#include "abstractagent.h"

osnmp::AbstractAgent::AbstractAgent(QObject *parent) : QObject(parent), mib_(nullptr), conf_(nullptr)
{

}

osnmp::AbstractAgent::~AbstractAgent()
{
    if(mib_ != nullptr) delete mib_;
    if(conf_ != nullptr) delete conf_;
}

void osnmp::AbstractAgent::setConf(osnmp::SnmpConf *value)
{
    if(conf_ != nullptr)
        delete conf_;
    conf_ = value;
}

void osnmp::AbstractAgent::setMib(osnmp::MibModule *value)
{
    if(mib_ != nullptr)
        delete mib_;
    mib_ = value;
}




