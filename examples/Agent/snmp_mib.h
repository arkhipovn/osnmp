#ifndef SNMP_MIB_H
#define SNMP_MIB_H

#include "osnmp/oid.h"
#include "ssu.h"

#define     VCH_MODULE_OID                  OID(OID::Enterprises) + 9191
#define     VCH003_MODULE_OID               VCH_MODULE_OID + 1

// vch003 / VCH003_MODULE_OID
#define     DEVICE_OID                      OID("1")
#define     INVENTORY_TABLE_OID             OID("2.1")  // inventoryTable.slotEntry
#define     SLOTS_OID                       OID("3")

// device / VCH003_MODULE_OID.DEVICE_OID
#define     DEVICE_NAME_OID                 "1"
#define     DEVICE_THRESHOLDS_OID           "2"
#define     DEVICE_NETWORK_OID              "5"
#define     DEVICE_POWER_OID                "6"

// deviceNetwork / VCH003_MODULE_OID.DEVICE_OID.DEVICE_NETWORK_OID
#define     DEVICE_NETWORK_INIT             "100"
#define     DEVICE_NETWORK_SAVE             "101"

#define     DEVICE_IP_ADDRESS_OID           "1"
#define     DEVICE_MASK_OID                 "2"

#define     DEVICE_NETWORK_SNMP                 OID("5")
#define     DEVICE_SNMP_TRAP_ENABLE             DEVICE_NETWORK_SNMP + "1"
#define     DEVICE_SNMP_ADD_TRAP_TARGET_IP      DEVICE_NETWORK_SNMP + "2"
#define     DEVICE_SNMP_DEL_TRAP_TARGET_IP      DEVICE_NETWORK_SNMP + "3"




// devicePower / VCH003_MODULE_OID.DEVICE_OID.DEVICE_POWER_OID
#define     DEVICE_POWER_CARD_A_OID         OID("1")
#define     DEVICE_POWER_CARD_B_OID         OID("2")

#define     DEVICE_POWER_STATUS_A           DEVICE_POWER_CARD_A_OID + 1
#define     DEVICE_POWER_FUSE_A             DEVICE_POWER_CARD_A_OID + 2

#define     DEVICE_POWER_STATUS_B           DEVICE_POWER_CARD_B_OID + 1
#define     DEVICE_POWER_FUSE_B             DEVICE_POWER_CARD_B_OID + 2

// slots        / VCH003_MODULE_OID.SLOTS_OID
#define     CARD_SLOT_OID(slot)             QString::number(slot + 1)


#define     CURRENT_TYPE_CARD_OID(card)     OID(QString::number(card))


#define     CHANNEL_NUMBER_OID              "1"
#define     CHANNEL_BAN_OID                 "2"
#define     CARD_TYPE_OID                   "100"
#define     INPUT_CHANNEL_TABLE_OID         "200.1"     // InChannelTable.InCnannelEntry
#define     OUTPUT_CHANNEL_TABLE_OID        "201.1"     // OutChannelTable.OutCnannelEntry


// Traps
#define     TRAPS_OID                       VCH003_MODULE_OID + 50

// Trap Values
#define     TRAP_VALUES                     TRAPS_OID + 1000

#define     TRAP_SLOT_NUMBER_VALUE          TRAP_VALUES + 1
#define     TRAP_POWER_NUMBER_VALUE         TRAP_VALUES + 2
#define     TRAP_CHANNEL_NUMBER_VALUE       TRAP_VALUES + 3

#define     TRAP_DEVICE                     TRAP_VALUES + 20
#define     TRAP_CARD                       TRAP_VALUES + 21

#define     TRAP_INPUT_CARD                 TRAP_CARD + CURRENT_TYPE_CARD_OID(SSU::Input)
#define     TRAP_GENERATOR_CARD             TRAP_CARD + CURRENT_TYPE_CARD_OID(SSU::Generator)
#define     TRAP_OUTPUT_CARD                TRAP_CARD + CURRENT_TYPE_CARD_OID(SSU::Output)

#define     TRAP_CARD_IN_VALUE              TRAP_DEVICE + 1
#define     TRAP_CARD_ACTIVE_VALUE          TRAP_DEVICE + 2
#define     TRAP_POWER_STATUS_VALUE         TRAP_DEVICE + 3
#define     TRAP_POWER_FUSE_VALUE           TRAP_DEVICE + 4

//#define     TRAP_INPUT_CARD_IN_CHANNEL_DISQUALIFICATION_VALUE           TRAP_INPUT_CARD + 1
#define     TRAP_GENERATOR_MODE_VALUE                                   TRAP_GENERATOR_CARD + 1


// Trap Notifications
#define     TRAP_CARD_IN_TRAP                   TRAPS_OID + 1
#define     TRAP_CARD_ACTIVE_TRAP               TRAPS_OID + 2
#define     TRAP_POWER_STATUS_TRAP              TRAPS_OID + 3
#define     TRAP_POWER_FUSE_TRAP                TRAPS_OID + 4

// Input Card Notifications
//#define     TRAP_INPUT_CARD_IN_CHANNEL_DISQUALIFICATION_TRAP           TRAPS_OID + 100


// Generator Notifications
#define     TRAP_GENERATOR_MODE_TRAP            TRAPS_OID + 400






enum InventoryTableVariable {
    kCardSlotColumn,
    kCardTypeColumn,
    kCardSerialNumberColumn,
    kCardSwVersionColumn,
    kCardHwVersionColumn,
};

enum SlotVariable {             // Общие для всех слотов параметры
    kCardType = 0
};

enum MonitoringCardVariable {   // General Information  / Monitoring card
    kDeviceName = kCardType + 1,
    kDeviceThresholds,

    //Network
    kDeviceIpAddr,
    kDeviceMask,
    kDeviceNetworkInit,
    kDeviceSaveConfig,

    // SNMP
    kDeviceSnmpTrapEnable,
    kDeviceSnmpAddTrapTargetIpAddr,
    kDeviceSnmpDelTrapTargetIpAddr,

    // Power
    kDevicePowerStatus1,
    kDevicePowerStatus2,
    kDeviceFuse1,
    kDeviceFuse2
};

enum InputCardVariable {
    kInputCardInChannelNumber = kCardType + 1,
    kInputCardInChannelBan,
    kInputCardInChannelType,
    kInputCardInChannelStatus,
    kInputCardInChannelStatusThreshold,
    kInputCardInChannelSsm,
    kInputCardInChannelSsmThreshold,
    kInputCardInChannelWaitToRestoreTime,
    kInputCardInChannelState,
};

enum GeneratorVariable {
    kGeneratorState = kCardType + 1,
    kGeneratorMode,
    kGeneratorManualChannelNumber,
    kGeneratorActiveChannelNumber,
    kGeneratorInChannelNumber,
    kGeneratorInChannelBan,
    kGeneratorInChannelPriority,
    kGeneratorInChannelCurrentPriority,
};

enum OutputCardVariable {
    kActiveChannelNumber = kCardType + 1,
    kBackupChannelNumber,
    kSynchronization500Hz,
    kOutputCardActiveMode,
    kOutputCardInChannelNumber,
    kOutputCardInChannelBan,
    kOutputCardInChannelPriority,
    kOutputCardInChannelCurrentPriority,

    kOutputCardOutChannelNumber,
    kOutputCardOutChannelType,
    kOutputCardOutChannelSsm,
    kOutputCardOutChannelSsmControlMode


};


#endif // SNMP_MIB_H
