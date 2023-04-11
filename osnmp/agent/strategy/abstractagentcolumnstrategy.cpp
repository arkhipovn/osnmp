#include "AbstractAgentColumnStrategy.h"

#ifdef QOBJECT_AGENT_STRATEGY
osnmp::AbstractAgentColumnStrategy::AbstractAgentColumnStrategy(int rows, QObject *parent) : AbstractAgentTableStrategy(parent), rows(rows)
{

}
#else
osnmp::AbstractAgentColumnStrategy::AbstractAgentColumnStrategy(int rows) : AbstractAgentTableStrategy(), rows(rows)
{

}
#endif

OID osnmp::AbstractAgentColumnStrategy::findTableRow(int valueId, const OID &index, bool exact) const
{
    Q_UNUSED(valueId)
    if(rows == 0) return OID::Null;

    int row = 0;
    if(exact) {
        if(index.isEmpty() || index.size() > 1) return OID::Null;
        row = index.at(0);
    }
    else {
        if(index.isEmpty()) return "1"; // Индекс начинается с 1
        row = index.at(0) + 1;  // следующий элемент
    }

    if(row < 1 || row > rows) return OID::Null;
    return QString::number(row);
}

OID osnmp::AbstractAgentColumnStrategy::lastTableRow(int valueId) const
{
    Q_UNUSED(valueId)
    return QString::number(rows + 1);   // Индекс начинается с 1
}
