#include "abstractagenttablestrategy.h"

#ifdef QOBJECT_AGENT_STRATEGY
osnmp::AbstractAgentTableStrategy::AbstractAgentTableStrategy(QObject *parent) : AbstractAgentStrategy(parent)
{

}
#endif

#ifndef QOBJECT_AGENT_STRATEGY
osnmp::AbstractAgentTableStrategy::AbstractAgentTableStrategy() : AbstractAgentStrategy()
{

}
#endif

OID osnmp::AbstractAgentTableStrategy::firstTableRow(int valueId) const
{
     return findTableRow(valueId, OID::Null, false);
}
