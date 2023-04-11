#ifndef TEST_H
#define TEST_H

#include "osnmp/agent/strategy/abstractagenttablestrategy.h"
#include "osnmp/agent/strategy/abstractagentcolumnstrategy.h"
#include "osnmp/conf/snmpconf.h"

#include "inventory.h"
#include "cards/generatorcard.h"
#include "cards/monitoringcard.h"
#include "cards/inputcard.h"
#include "cards/outputcard.h"

#include "snmp_mib.h"


class InventoryTableAgentStrategy : public osnmp::AbstractAgentTableStrategy {
public:
    InventoryTableAgentStrategy(Inventory *inventory);

    OID findTableRow(int valueId, const OID &index, bool exact) const override;
    OID lastTableRow(int valueId) const override;

    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) override;
    osnmp:: Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode) override;

private:
    Inventory *inventory;
};

// ##########################

class MonitoringCardAgentStrategy : public osnmp::AbstractAgentStrategy {
public:
    MonitoringCardAgentStrategy(int slot, Inventory *inventory, osnmp::SnmpConf *conf);
    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) override;
    osnmp:: Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode) override;
private:
    int slot;
    Inventory *inventory;
    osnmp::SnmpConf *conf;

    bool networkInit;
    QHostAddress newIpAddress;
    QHostAddress newMask;
};

// ##########################

class InputCardAgentStrategy : public osnmp::AbstractAgentColumnStrategy {
public:
    InputCardAgentStrategy(int slot, Inventory *inventory, int inputChannels);
    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) override;
    osnmp:: Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode) override;
private:
    int slot;
    Inventory *inventory;
};

// ##########################


class GeneratorAgentStrategy : public osnmp::AbstractAgentColumnStrategy {
public:
    GeneratorAgentStrategy(int slot, Inventory *inventory, int inputChannels);
    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) override;
    osnmp:: Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode) override;
private:
    int slot;
    Inventory *inventory;
};

// ##########################

class OutputCardAgentStrategy : public osnmp::AbstractAgentColumnStrategy {
public:
    OutputCardAgentStrategy(int slot, Inventory *inventory, int inputChannels);
    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) override;
    osnmp:: Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode) override;
private:
    int slot;
    Inventory *inventory;
};

class OutputCardOutChannelTableStrategy : public osnmp::AbstractAgentColumnStrategy {
public:
    OutputCardOutChannelTableStrategy(int slot, Inventory *inventory, int outputChannels);
    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) override;
    osnmp:: Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode) override;
private:
    int slot;
    Inventory *inventory;
};



#endif // TEST_H
