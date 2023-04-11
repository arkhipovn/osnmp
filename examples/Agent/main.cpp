#include <QCoreApplication>

#include "osnmp/agent/udpagent.h"
#include "agentstrategy.h"
#include <QTimer>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    using namespace osnmp;

    Inventory inventory;

    //############## Inventory init ###################
    auto in1 = new InputCard;
    in1->setSerialNumber("in_sn1");
    in1->setHardware("in_hw1");
    in1->setSoftware("in_sw1");


    auto in2 = new InputCard;
    in2->setSerialNumber("in_sn2");
    in2->setHardware("in_hw2");
    in2->setSoftware("in_sw2");


    auto bm = new MonitoringCard;
    bm->setSerialNumber("bm_sn1");
    bm->setHardware("bm_hw1");
    bm->setSoftware("bm_sw1");
    bm->setName("DeviceName");
    bm->setDeviceThresholdsEnabled(false);
    bm->setIp("192.168.124.95");
    bm->setPort(5005);
    bm->setMask("255.255.255.255");
    bm->setPowerCard1({false, true});
    bm->setPowerCard2({true, false});


    auto g1 = new GeneratorCard;
    g1->setSerialNumber("gen_sn1");
    g1->setHardware("gen_hw1");
    g1->setSoftware("gen_sw1");
    g1->setActiveChannel(1);
    g1->setGeneratorState(SSU::Warmup);
    g1->setGeneratorMode(SSU::GModeAuto);
    for(int i = 0; i < g1->numberOfInputs(); ++i) {
        g1->setBan(i, false);
        g1->setCurrentPriority(i, i % 8);
        g1->setPriority(i, i % 8);
    }

    auto g2 = new GeneratorCard;
    g2->setSerialNumber("gen_sn2");
    g2->setHardware("gen_hw2");
    g2->setSoftware("gen_sw2");
    g2->setActiveChannel(5);
    g2->setGeneratorState(SSU::Freerun);
    g2->setGeneratorMode(SSU::GModeForcedHoldover);
    g2->setManualChannel(5);
    for(int i = 0; i < g2->numberOfInputs(); ++i) {
        g2->setBan(i, true);
        g2->setCurrentPriority(i, i % 8);
        g2->setPriority(i, i % 8);
    }

    auto out1 = new OutputCard;
    out1->setSerialNumber("out_sn2");
    out1->setHardware("out_hw2");
    out1->setSoftware("out_sw2");



    inventory.append(0, in1);
    inventory.append(1, in2);
    inventory.append(6, g1);
    inventory.append(8, g2);
    inventory.append(10, out1);
    inventory.append(MONITORING_CARD_SLOT, bm);

    //#################################







    auto conf = new SnmpConf;
    conf->communities().insert("user", { ObjectAccess::ReadOnly, { VCH003_MODULE_OID + DEVICE_OID + DEVICE_NAME_OID,
                                                                   VCH003_MODULE_OID + DEVICE_OID + DEVICE_POWER_OID,
                                                                   VCH003_MODULE_OID + INVENTORY_TABLE_OID,
                                                                   VCH003_MODULE_OID + SLOTS_OID } });
    conf->communities().insert("public", { ObjectAccess::ReadWrite, { VCH003_MODULE_OID + DEVICE_OID + DEVICE_NAME_OID,
                                                                      VCH003_MODULE_OID + DEVICE_OID + DEVICE_POWER_OID,
                                                                      VCH003_MODULE_OID + INVENTORY_TABLE_OID,
                                                                      VCH003_MODULE_OID + SLOTS_OID } });
    conf->communities().insert("sec", { ObjectAccess::ReadWrite, {}});   // Доступно все


    TrapSettings trapSettings;
    trapSettings.setCommunity("public");
    trapSettings.setPort(5005);
    trapSettings.appendTrapTarget(QHostAddress::LocalHost);

    conf->setTrapSettings(trapSettings);





    auto mib = new MibModule(VCH003_MODULE_OID);

    //#### Inventory table ####
    auto inventoryTable = new MibTableObject(ObjectAccess::NoAccess, INVENTORY_TABLE_OID, new InventoryTableAgentStrategy(&inventory));
    inventoryTable->append({
                               new MibObject(kCardSlotColumn,           Type::Integer,      ObjectAccess::ReadOnly, "1"),
                               new MibObject(kCardTypeColumn,           Type::Integer,      ObjectAccess::ReadOnly, "2"),
                               new MibObject(kCardSerialNumberColumn,   Type::OctetString,  ObjectAccess::ReadOnly, "3"),
                               new MibObject(kCardSwVersionColumn,      Type::OctetString,  ObjectAccess::ReadOnly, "4"),
                               new MibObject(kCardHwVersionColumn,      Type::OctetString,  ObjectAccess::ReadOnly, "5")
                           });
    mib->append(inventoryTable);


    //#### General Information  / A18 / Monitoring card ####
    auto deviceModule = new MibObjectIdentifier(DEVICE_OID, new MonitoringCardAgentStrategy(MONITORING_CARD_SLOT, &inventory, conf));
    mib->append(deviceModule);

    deviceModule->append({
                             new MibObject(kDeviceName,            Type::OctetString,  ObjectAccess::ReadWrite,    DEVICE_NAME_OID),
                             new MibObject(kDeviceThresholds,      Type::Integer,      ObjectAccess::ReadWrite,    DEVICE_THRESHOLDS_OID),
                         });

    auto deviceNetworkModule = new MibObjectIdentifier(DEVICE_NETWORK_OID);
    deviceModule->append(deviceNetworkModule);
    deviceNetworkModule->append({
                                    new MibObject(kDeviceIpAddr,                Type::IpAddress,    ObjectAccess::ReadWrite,    DEVICE_IP_ADDRESS_OID ),
                                    new MibObject(kDeviceMask,                  Type::IpAddress,    ObjectAccess::ReadWrite,    DEVICE_MASK_OID ),

                                    new MibObject(kDeviceNetworkInit,           Type::Integer,    ObjectAccess::ReadWrite,    DEVICE_NETWORK_INIT ),
                                    new MibObject(kDeviceSaveConfig,            Type::Integer,    ObjectAccess::ReadWrite,    DEVICE_NETWORK_SAVE ),

                                    new MibObject(kDeviceSnmpTrapEnable,           Type::Integer,    ObjectAccess::ReadWrite,       DEVICE_SNMP_TRAP_ENABLE ),
                                    new MibObject(kDeviceSnmpAddTrapTargetIpAddr,  Type::IpAddress,    ObjectAccess::ReadWrite,     DEVICE_SNMP_ADD_TRAP_TARGET_IP ),
                                    new MibObject(kDeviceSnmpDelTrapTargetIpAddr,  Type::IpAddress,    ObjectAccess::ReadWrite,     DEVICE_SNMP_DEL_TRAP_TARGET_IP ),
                                });



    auto devicePowerModule = new MibObjectIdentifier(DEVICE_POWER_OID);
    deviceModule->append(devicePowerModule);
    devicePowerModule->append({
                                  new MibObject(kDevicePowerStatus1,    Type::Integer,      ObjectAccess::ReadOnly,     DEVICE_POWER_STATUS_A ),
                                  new MibObject(kDevicePowerStatus2,    Type::Integer,      ObjectAccess::ReadOnly,     DEVICE_POWER_STATUS_B ),
                                  new MibObject(kDeviceFuse1,           Type::Integer,      ObjectAccess::ReadOnly,     DEVICE_POWER_FUSE_A ),
                                  new MibObject(kDeviceFuse2,           Type::Integer,      ObjectAccess::ReadOnly,     DEVICE_POWER_FUSE_B )
                              });

    //#### Cards ####

    auto slotsModule = new MibObjectIdentifier(SLOTS_OID);
    mib->append(slotsModule);

    // A00 - A03. Input card
    for(int i = 0; i < 4; ++i) {
        auto slot = new MibObjectIdentifier(CARD_SLOT_OID(i), new InputCardAgentStrategy(i, &inventory, NUMBER_OF_INPUT_CARD_INPUTS));
        slotsModule->append(slot);
        slot->append(new MibObject(kCardType, Type::Integer, ObjectAccess::ReadOnly, CARD_TYPE_OID ));

        // inputs channels
        auto inputCardInCnannelTable = new MibTableObject(ObjectAccess::NoAccess, CURRENT_TYPE_CARD_OID(SSU::Input) + INPUT_CHANNEL_TABLE_OID);
        slot->append(inputCardInCnannelTable);
        inputCardInCnannelTable->append({
                                            new MibObject(kInputCardInChannelNumber,             Type::Integer,  ObjectAccess::ReadOnly,     CHANNEL_NUMBER_OID),
                                            new MibObject(kInputCardInChannelType,               Type::Integer,  ObjectAccess::ReadWrite,    "2"),
                                            new MibObject(kInputCardInChannelStatus,             Type::Integer,  ObjectAccess::ReadOnly,     "3"),
                                            new MibObject(kInputCardInChannelStatusThreshold,    Type::Integer,  ObjectAccess::ReadWrite,    "4"),
                                            new MibObject(kInputCardInChannelSsm,                Type::Integer,  ObjectAccess::ReadOnly,     "5"),
                                            new MibObject(kInputCardInChannelSsmThreshold,       Type::Integer,  ObjectAccess::ReadWrite,    "6"),
                                            new MibObject(kInputCardInChannelWaitToRestoreTime,  Type::Integer,  ObjectAccess::ReadOnly,     "7"),
                                            new MibObject(kInputCardInChannelState,              Type::Integer,  ObjectAccess::ReadOnly,     "8")
                                        });
    }


    // A06. Generator

    for(int i = 0; i < 2; ++i) {
        auto slot = new MibObjectIdentifier(CARD_SLOT_OID(6 + i * 2), new GeneratorAgentStrategy(6 + i * 2, &inventory, NUMBER_OF_GENERATOR_CARD_INPUTS));
        slotsModule->append(slot);
        slot->append(new MibObject(kCardType, Type::Integer, ObjectAccess::ReadOnly, CARD_TYPE_OID ));

        auto generatorCard = new MibObjectIdentifier(CURRENT_TYPE_CARD_OID(SSU::Generator));
        slot->append(generatorCard);
        generatorCard->append({
                                  new MibObject(kGeneratorState, Type::Integer, ObjectAccess::ReadOnly, "1"),
                                  new MibObject(kGeneratorMode, Type::Integer, ObjectAccess::ReadWrite, "2"),
                                  new MibObject(kGeneratorManualChannelNumber, Type::Integer, ObjectAccess::ReadWrite, "3"),
                                  new MibObject(kGeneratorActiveChannelNumber, Type::Integer, ObjectAccess::ReadOnly, "4"),
                              });

        //inputs channels
        auto generatorCardInCnannelTable = new MibTableObject(ObjectAccess::NoAccess, INPUT_CHANNEL_TABLE_OID);
        generatorCard->append(generatorCardInCnannelTable);
        generatorCardInCnannelTable->append({
                                                new MibObject(kGeneratorInChannelNumber, Type::Integer, ObjectAccess::ReadOnly, CHANNEL_NUMBER_OID),
                                                new MibObject(kGeneratorInChannelBan, Type::Integer, ObjectAccess::ReadOnly, CHANNEL_BAN_OID),
                                                new MibObject(kGeneratorInChannelPriority, Type::Integer, ObjectAccess::ReadWrite, "3"),
                                                new MibObject(kGeneratorInChannelCurrentPriority, Type::Integer, ObjectAccess::ReadOnly, "4"),
                                            });
    }



    // A10-A11. Output card
    for(int i = 10; i < 12; ++i) {
        auto slot = new MibObjectIdentifier(CARD_SLOT_OID(i), new OutputCardAgentStrategy(i, &inventory, NUMBER_OF_OUTPUT_CARD_INPUTS));
        slotsModule->append(slot);
        slot->append(new MibObject(kCardType, Type::Integer, ObjectAccess::ReadOnly, CARD_TYPE_OID ));

        auto outputCard = new MibObjectIdentifier(CURRENT_TYPE_CARD_OID(SSU::OutputH));
        slot->append(outputCard);

        outputCard->append({
                               new MibObject(kActiveChannelNumber, Type::Integer, ObjectAccess::ReadOnly, "1"),
                               new MibObject(kBackupChannelNumber, Type::Integer, ObjectAccess::ReadOnly, "2"),
                               new MibObject(kSynchronization500Hz, Type::Integer, ObjectAccess::ReadOnly, "3"),
                               new MibObject(kOutputCardActiveMode, Type::Integer, ObjectAccess::ReadWrite, "4"),
                           });

        //inputs channels
        auto outputCardInCnannelTable = new MibTableObject(ObjectAccess::NoAccess, INPUT_CHANNEL_TABLE_OID);
        outputCard->append(outputCardInCnannelTable);
        outputCardInCnannelTable->append({
                                             new MibObject(kOutputCardInChannelNumber, Type::Integer, ObjectAccess::ReadOnly, CHANNEL_NUMBER_OID),
                                             new MibObject(kOutputCardInChannelBan, Type::Integer, ObjectAccess::ReadOnly, CHANNEL_BAN_OID),
                                             new MibObject(kOutputCardInChannelPriority, Type::Integer, ObjectAccess::ReadWrite, "3"),
                                             new MibObject(kOutputCardInChannelCurrentPriority, Type::Integer, ObjectAccess::ReadOnly, "4"),
                                         });



        auto outputCardOutCnannelTable = new MibTableObject(ObjectAccess::NoAccess, OUTPUT_CHANNEL_TABLE_OID, new OutputCardOutChannelTableStrategy(i, &inventory, NUMBER_OF_OUTPUT_CARD_OUTPUTS));
        outputCard->append(outputCardOutCnannelTable);
        outputCardOutCnannelTable->append({
                                              new MibObject(kOutputCardInChannelNumber, Type::Integer, ObjectAccess::ReadOnly, CHANNEL_NUMBER_OID),
                                              new MibObject(kOutputCardOutChannelType, Type::Integer, ObjectAccess::ReadWrite, "2"),
                                              new MibObject(kOutputCardOutChannelSsm, Type::Integer, ObjectAccess::ReadWrite, "3"),
                                              new MibObject(kOutputCardOutChannelSsmControlMode, Type::Integer, ObjectAccess::ReadWrite, "4"),
                                          });
    }


    // A12-A17
    for(int i = 12; i < 18; ++i) {
        auto slot = new MibObjectIdentifier(CARD_SLOT_OID(i), new OutputCardAgentStrategy(i, &inventory, NUMBER_OF_OUTPUT_CARD_INPUTS));
        slotsModule->append(slot);
        slot->append(new MibObject(kCardType, Type::Integer, ObjectAccess::ReadOnly, CARD_TYPE_OID ));  //OutputMultiplier
    }







    UdpAgent agent;
    agent.setMib(mib);
    agent.setConf(conf);
    agent.open(QHostAddress::LocalHost, 5000);



    // Trap example
    QTimer::singleShot(10000, [&]() {
        qDebug() << "Trap";
        using namespace osnmp;
        TrapV2 trap(conf->trapSettings( ).community(), TRAP_CARD_IN_TRAP);
        trap.addIntegerVar(TRAP_SLOT_NUMBER_VALUE, 4);
        trap.addIntegerVar(TRAP_CARD_IN_VALUE, 1);
        agent.sendTrap(trap);
    });




    return a.exec();
}





