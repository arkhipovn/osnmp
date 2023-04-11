#include "agentstrategy.h"

using namespace osnmp;
using namespace SSU;

InventoryTableAgentStrategy::InventoryTableAgentStrategy(Inventory *inventory) : AbstractAgentTableStrategy(), inventory(inventory)
{

}

OID InventoryTableAgentStrategy::findTableRow(int valueId, const OID &index, bool exact) const
{
    Q_UNUSED(valueId)
    if(exact) { // нужно найти точное совпадение
        if(index.isEmpty() || index.size() > 1) return OID::Null;  // Тк нужно точное совпадение..
        auto it = inventory->find(index.at(0) - 1);
        return it == inventory->end() ? OID::Null
                                      : index;   // Тк нужно точное совпадение, то вернем этот же индекс
    }
    else {
        // нужно найти следующий элемент
        auto it = index.isEmpty() ? inventory->begin()
                                  : inventory->upperBound(index.at(0) - 1);
        return it == inventory->end() ? OID::Null
                                      : OID(QString::number(it.key() + 1));   // Вернем то, что нашли
    }
}

OID InventoryTableAgentStrategy::lastTableRow(int valueId) const
{
    Q_UNUSED(valueId)
    if(inventory->size() == 0) return OID::Null;
    auto it = inventory->end();
    --it;
    return QString::number(it.key());
}

osnmp::Variant InventoryTableAgentStrategy::getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode)
{
    Q_UNUSED(errorCode)

    auto it = inventory->find(index.at(0) - 1);
    if(it == inventory->end()) return NoSuchInstance();

    switch (static_cast<InventoryTableVariable>(valueId)) {
        //######## Inventory table
        case kCardSlotColumn:
            return static_cast<int>(index.at(0));
        case kCardTypeColumn:
            return static_cast<int>(it.value()->getType());
        case kCardSerialNumberColumn:
            return it.value()->getSerialNumber();
        case kCardSwVersionColumn:
            return it.value()->getSoftware();
        case kCardHwVersionColumn:
            return it.value()->getHardware();
        default:
            break;
    }

    // Если в данный момент данные не получится вернуть или установить, то установить NoSuchInstance
    return NoSuchInstance();
}

osnmp::Variant InventoryTableAgentStrategy::setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode)
{
    // Таблица не должна быть перезаписываемой
    Q_UNUSED(valueId)
    Q_UNUSED(index)
    Q_UNUSED(var)
    Q_UNUSED(errorCode)
    return NoSuchInstance();
}

// ##########################

MonitoringCardAgentStrategy::MonitoringCardAgentStrategy(int slot, Inventory *inventory, SnmpConf *conf) :
    AbstractAgentStrategy(), slot(slot), inventory(inventory), conf(conf), networkInit(false)
{

}

Variant MonitoringCardAgentStrategy::getValue(int valueId, const OID &index, ErrorCodes &errorCode)
{
    Q_UNUSED(errorCode)
    Q_UNUSED(index)

    auto it = inventory->find(slot);
    if(it == inventory->end()) {
        if(valueId == kCardType) return static_cast<int>(EmptySlot);    // В этом случае вернем значение 0
        else return Null(); //@N пока не придумал, что возвращать..
    }

    auto card = static_cast<MonitoringCard*>(it.value());

    switch (valueId) {
        case kCardType: return static_cast<int>(card->getType());
        case kDeviceName: return card->getName();
        case kDeviceThresholds: return card->getDeviceThresholdsEnabled() ? 1 : 0;
            // Network
        case kDeviceIpAddr: return QHostAddress(card->getIp());
        case kDeviceMask: return QHostAddress(card->getMask());


            //kDeviceSnmpDelTrapTargetIpAddr

        case kDeviceNetworkInit: return networkInit ? 1 : 0;
        case kDeviceSaveConfig: return 0;
        case kDeviceSnmpTrapEnable: return conf->trapSettings().enable() ? 1 : 0;
        case kDeviceSnmpAddTrapTargetIpAddr: return QHostAddress(QHostAddress::Null);
        case kDeviceSnmpDelTrapTargetIpAddr: return QHostAddress(QHostAddress::Null);

            // Power
        case kDevicePowerStatus1: return card->getPowerCard1().power ? 1 : 0;
        case kDevicePowerStatus2: return card->getPowerCard2().power ? 1 : 0;
        case kDeviceFuse1: return card->getPowerCard1().fuse ? 1 : 0;
        case kDeviceFuse2: return card->getPowerCard2().fuse ? 1 : 0;
        default: break;
    }


    return NoSuchInstance();
}

Variant MonitoringCardAgentStrategy::setValue(int valueId, const OID &index, const Variant &var, ErrorCodes &errorCode)
{
    Q_UNUSED(index)

    auto it = inventory->find(slot);
    if(it == inventory->end())
        return NoSuchInstance();

    auto card = static_cast<MonitoringCard*>(it.value());

    switch (valueId) {
        case kDeviceName: {
            card->setName(var);
            return var;
        }
        case kDeviceThresholds: {
            int value = var;
            if(value < 0 || value > 1) {
                errorCode = ErrorCodes::BadValue;
            }
            else card->setDeviceThresholdsEnabled(value);
            return card->getDeviceThresholdsEnabled() ? 1 : 0;
        }

        case kDeviceNetworkInit: {
            int value = var;
            if(value == 0 || value == 1) {
                // Начинаем инициализацию
                newIpAddress.clear();
                newMask.clear();
                networkInit = value;
            }
            else
                errorCode = ErrorCodes::BadValue;
            return networkInit ? 1 : 0;
        }

        case kDeviceSaveConfig: {
            int value = var;
            if(value == 0) {}
            else if(value == 1) {
                // Сохраняем конфиг
                if(!newIpAddress.isNull())
                    card->setIp(newIpAddress.toString());
                if(!newMask.isNull())
                    card->setMask(newMask.toString());
                newIpAddress.clear();
                newMask.clear();
                networkInit = false;
            }
            else
                errorCode = ErrorCodes::BadValue;
            return 1;
        }

        case kDeviceIpAddr: {
            if(networkInit) {
                // Запоминаем Ip
                newIpAddress = var_cast<QHostAddress>(var);
                return newIpAddress;
            }
            else
                errorCode = ErrorCodes::BadValue;
            return card->getIp();
        }
        case kDeviceMask: {
            if(networkInit) {
                // Запоминаем маску
                newMask = var_cast<QHostAddress>(var);
                return newMask;
            }
            else
                errorCode = ErrorCodes::BadValue;
            return card->getMask();
        }

        case kDeviceSnmpTrapEnable: {
            int value = var;
            if(value == 0 || value == 1)
                conf->trapSettings().setEnable(static_cast<bool>(value));
            else
                errorCode = ErrorCodes::BadValue;
            return conf->trapSettings().enable() ? 1 : 0;
        }

        case kDeviceSnmpAddTrapTargetIpAddr: {
            QHostAddress target = var_cast<QHostAddress>(var);
            if(!target.isNull())
                conf->trapSettings().appendTrapTarget(target);
            return var;
        }

        case kDeviceSnmpDelTrapTargetIpAddr: {
            QHostAddress target = var_cast<QHostAddress>(var);
            if(!target.isNull())
                conf->trapSettings().eraseTrapTarget(target);
            return QHostAddress(QHostAddress::Null);
        }

        default: break;
    }

    return NoSuchInstance();
}

// ##########################

InputCardAgentStrategy::InputCardAgentStrategy(int slot, Inventory *inventory, int inputChannels) : AbstractAgentColumnStrategy(inputChannels), slot(slot), inventory(inventory)
{

}

Variant InputCardAgentStrategy::getValue(int valueId, const OID &index, ErrorCodes &errorCode)
{
    Q_UNUSED(errorCode)

    auto it = inventory->find(slot);
    if(it == inventory->end()) {
        if(valueId == kCardType) return static_cast<int>(EmptySlot);    // В этом случае вернем значение 0
        else if(valueId != kInputCardInChannelNumber)
            return Null(); //@N пока не придумал, что возвращать..
    }

    auto card = static_cast<InputCard*>(it.value());

    switch (valueId) {
        case kCardType: return static_cast<int>(card->getType());
        case kInputCardInChannelNumber:
            return int(index.at(0));
        case kInputCardInChannelBan:
            return card->channelAt(index.at(0) - 1).ban ? 0 : 1;   // - 1 тк индекс начинается с 1
        case kInputCardInChannelType:
            return static_cast<int>(card->channelAt(index.at(0) - 1).type);
        case kInputCardInChannelStatus:
            return static_cast<int>(card->channelAt(index.at(0) - 1).status);
        case kInputCardInChannelStatusThreshold:
            return static_cast<int>(card->channelAt(index.at(0) - 1).statusThreshold);
        case kInputCardInChannelSsm:
            return static_cast<int>(card->channelAt(index.at(0) - 1).ssm);
        case kInputCardInChannelSsmThreshold:
            return static_cast<int>(card->channelAt(index.at(0) - 1).ssmThreshold);
        case kInputCardInChannelWaitToRestoreTime:
            return card->channelAt(index.at(0) - 1).waitToRestoreTime;
        case kInputCardInChannelState:
            return card->channelAt(index.at(0) - 1).state;
        default: break;
    }

    return NoSuchInstance();
}

Variant InputCardAgentStrategy::setValue(int valueId, const OID &index, const Variant &var, ErrorCodes &errorCode)
{
    auto it = inventory->find(slot);
    if(it == inventory->end())
        return NoSuchInstance();

    auto card = static_cast<InputCard*>(it.value());

    switch (valueId) {
        case kInputCardInChannelType: {
            int type = var;
            switch (type) {
                case SignalTypeOff:
                case SignalType10MHz:
                case SignalType2048KHz:
                case SignalTypeE1:
                    card->setType(index.at(0) - 1, static_cast<SSU::SignalType>(type));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return static_cast<int>(card->channelAt(index.at(0) - 1).type);
        }
        case kInputCardInChannelStatusThreshold: {
            int threshold = var;
            switch (threshold) {
                case StatusThresholdNone:
                case StatusThresholdLow:
                case StatusThresholdHigh:
                    card->setStatusThreshold(index.at(0) - 1, static_cast<SSU::StatusThreshold>(threshold));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return static_cast<int>(card->channelAt(index.at(0) - 1).statusThreshold);
        }
        case kInputCardInChannelSsmThreshold: {
            int threshold = var;
            switch (threshold) {
                case NONE:
                case PRC:
                case SSU_T:
                case SSU_L:
                case SEC:
                case DNU:
                    card->setSsmThreshold(index.at(0) - 1, static_cast<SSU::SSM>(threshold));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return static_cast<int>(card->channelAt(index.at(0) - 1).ssmThreshold);
        }
        case kInputCardInChannelWaitToRestoreTime: {
            int sec = var;
            if(sec < 0 || sec > 1275) {
                errorCode = ErrorCodes::BadValue;
            }
            else card->setWaitToRestoreTime(index.at(0) - 1, sec);
            return card->channelAt(index.at(0) - 1).waitToRestoreTime;
        }
        default: break;
    }

    return NoSuchInstance();
}

// ##########################

GeneratorAgentStrategy::GeneratorAgentStrategy(int slot, Inventory *inventory, int inputChannels) :
    AbstractAgentColumnStrategy(inputChannels), slot(slot), inventory(inventory)
{

}

Variant GeneratorAgentStrategy::getValue(int valueId, const OID &index, ErrorCodes &errorCode)
{
    Q_UNUSED(errorCode)

    auto it = inventory->find(slot);
    if(it == inventory->end()) {
        if(valueId == kCardType) return static_cast<int>(EmptySlot);    // В этом случае вернем значение 0
        else if(valueId != kGeneratorInChannelNumber)
            return Null(); //@N пока не придумал, что возвращать..
    }

    auto card = static_cast<GeneratorCard*>(it.value());

    switch (valueId) {
        case kCardType: return static_cast<int>(card->getType());
        case kGeneratorState: return int(card->getGeneratorState());
        case kGeneratorMode: return int(card->getGeneratorMode());
        case kGeneratorManualChannelNumber: {
            if(card->getGeneratorMode() == GModeManual)
                return card->getManualChannel() + 1;
            else return Null(); //@N пока не придумал, что возвращать..
        }
        case kGeneratorActiveChannelNumber:
            return card->getActiveChannel() + 1;
            // Generator inputs table
        case kGeneratorInChannelNumber:
            return int(index.at(0));
        case kGeneratorInChannelPriority:
            return card->channelAt(index.at(0) - 1).priority;     // - 1 тк индекс начинается с 1
        case kGeneratorInChannelCurrentPriority:
            return card->channelAt(index.at(0) - 1).currentPriority;   // - 1 тк индекс начинается с 1
        case kGeneratorInChannelBan:
            return card->channelAt(index.at(0) - 1).ban ? 0 : 1;   // - 1 тк индекс начинается с 1
        default: break;
    }
    return NoSuchInstance();
}

Variant GeneratorAgentStrategy::setValue(int valueId, const OID &index, const Variant &var, ErrorCodes &errorCode)
{
    auto it = inventory->find(slot);
    if(it == inventory->end()) return NoSuchInstance();

    auto card = static_cast<GeneratorCard*>(it.value());

    switch (valueId) {
        case kGeneratorMode: {
            int mode = var;
            switch (mode) {
                case GModeAuto:
                case GModeManual:
                case GModeForcedHoldover:
                    card->setGeneratorMode(static_cast<SSU::GeneratorMode>(mode));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return int(card->getGeneratorMode());
        }
        case kGeneratorManualChannelNumber: {
            if(card->getGeneratorMode() != GModeManual)
                errorCode = ErrorCodes::NotWritale;
            else {
                int num = var;
                if(num < 0 || num > 18) {
                    errorCode = ErrorCodes::BadValue;
                }
                else card->setManualChannel(num);
            }
            return card->getManualChannel();
        }
        case kGeneratorInChannelPriority: {
            int sec = var;
            if(sec < 0 || sec > 9) {
                errorCode = ErrorCodes::BadValue;
            }
            else {
                //@N
                //                card->setPriority(index.at(0) - 1, ...);
            }
            return card->channelAt(index.at(0) - 1).priority;     // - 1 тк индекс начинается с 1
        }
        default: break;
    }

    return NoSuchInstance();
}

// ##########################

OutputCardAgentStrategy::OutputCardAgentStrategy(int slot, Inventory *inventory, int inputChannels) :
    AbstractAgentColumnStrategy(inputChannels), slot(slot), inventory(inventory)
{

}

Variant OutputCardAgentStrategy::getValue(int valueId, const OID &index, ErrorCodes &errorCode)
{
    Q_UNUSED(errorCode)

    auto it = inventory->find(slot);
    if(it == inventory->end()) {
        if(valueId == kCardType) return static_cast<int>(EmptySlot);    // В этом случае вернем значение 0
        else if(valueId != kOutputCardInChannelNumber)
            return Null(); //@N пока не придумал, что возвращать..
    }

    auto card = static_cast<OutputCard*>(it.value());

    switch (valueId) {
        case kCardType:
            return static_cast<int>(card->getType());
        case kActiveChannelNumber:
            return card->getActiveChannel();
        case kBackupChannelNumber:
            return card->getBackupChannel();
        case kSynchronization500Hz:
            return card->getSynchronization() ? 1 : 0;
        case kOutputCardActiveMode:
            return static_cast<int>(card->getMode());
        case kOutputCardInChannelNumber:
            return int(index.at(0));
        case kOutputCardInChannelPriority:
            if(index.at(0) > NUMBER_OF_OUTPUT_CARD_INPUTS - NUMBER_OF_GENERATOR_CARD)   // Нельзя задавать приоритеты для сигналов генератора
                return Null();
            return card->inputChannelAt(index.at(0) - 1).priority;     // - 1 тк индекс начинается с 1
        case kOutputCardInChannelCurrentPriority:
            if(index.at(0) > NUMBER_OF_OUTPUT_CARD_INPUTS - NUMBER_OF_GENERATOR_CARD)   // Нельзя задавать приоритеты для сигналов генератора
                return Null();
            return card->inputChannelAt(index.at(0) - 1).currentPriority;   // - 1 тк индекс начинается с 1
        case kOutputCardInChannelBan:
            return card->inputChannelAt(index.at(0) - 1).ban ? 0 : 1;   // - 1 тк индекс начинается с 1
        default: break;
    }
    return NoSuchInstance();
}

Variant OutputCardAgentStrategy::setValue(int valueId, const OID &index, const Variant &var, ErrorCodes &errorCode)
{
    auto it = inventory->find(slot);
    if(it == inventory->end()) return NoSuchInstance();

    auto card = static_cast<OutputCard*>(it.value());

    switch (valueId) {
        case kOutputCardActiveMode: {
            int mode = var;
            switch (mode) {
                case Active:
                case Passive:
                    card->setMode(static_cast<SSU::CardMode>(mode));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return static_cast<int>(card->getMode());
        }
        case kOutputCardInChannelPriority: {
            if(index.at(0) > NUMBER_OF_OUTPUT_CARD_INPUTS - NUMBER_OF_GENERATOR_CARD) { // Нельзя задавать приоритеты для сигналов генератора
                errorCode = ErrorCodes::BadValue;
            }
            int sec = var;
            if(sec < 0 || sec > 9) {
                errorCode = ErrorCodes::BadValue;
            }
            else {
                //@N
                //                card->setPriority(index.at(0) - 1, ...);
            }
            return card->inputChannelAt(index.at(0) - 1).priority;     // - 1 тк индекс начинается с 1
        }
        default: break;
    }

    return NoSuchInstance();
}

// ##########################


OutputCardOutChannelTableStrategy::OutputCardOutChannelTableStrategy(int slot, Inventory *inventory, int outputChannels) :
    AbstractAgentColumnStrategy(outputChannels), slot(slot), inventory(inventory)
{

}

Variant OutputCardOutChannelTableStrategy::getValue(int valueId, const OID &index, ErrorCodes &errorCode)
{
    Q_UNUSED(errorCode)

    auto it = inventory->find(slot);
    if(it == inventory->end() && valueId != kOutputCardInChannelNumber)
        return Null(); //@N пока не придумал, что возвращать..

    auto card = static_cast<OutputCard*>(it.value());

    switch (valueId) {
        case kOutputCardInChannelNumber:
            return int(index.at(0));
        case kOutputCardOutChannelType:
            return static_cast<int>(card->outputChannelAt(index.at(0) - 1).type);
        case kOutputCardOutChannelSsm:
            return static_cast<int>(card->outputChannelAt(index.at(0) - 1).ssm);
        case kOutputCardOutChannelSsmControlMode:
            return static_cast<int>(card->outputChannelAt(index.at(0) - 1).ssmControlMode);
        default: break;
    }
    return NoSuchInstance();
}

Variant OutputCardOutChannelTableStrategy::setValue(int valueId, const OID &index, const Variant &var, ErrorCodes &errorCode)
{
    auto it = inventory->find(slot);
    if(it == inventory->end())
        return NoSuchInstance();

    auto card = static_cast<OutputCard*>(it.value());

    switch (valueId) {
        case kOutputCardOutChannelType: {
            int type = var;
            switch (type) {
                case SignalTypeOff:
                case SignalType2048KHz:
                case SignalTypeE1:
                    card->setType(index.at(0) - 1, static_cast<SSU::SignalType>(type));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return static_cast<int>(card->outputChannelAt(index.at(0) - 1).type);
        }
        case kOutputCardOutChannelSsm: {
            if(card->outputChannelAt(index.at(0) - 1).ssmControlMode != SSU::ssmModeMANUAL) {   // Если мы не в ручном режиме, то нельзя задавать
                errorCode = ErrorCodes::NotWritale;
            }
            else {
                int ssm = var;
                switch (ssm) {
                    case NONE:
                    case PRC:
                    case SSU_T:
                    case SSU_L:
                    case SEC:
                    case DNU:
                        card->setSsm(index.at(0) - 1, static_cast<SSU::SSM>(ssm));
                        break;
                    default:
                        errorCode = ErrorCodes::BadValue;
                        break;
                }
            }
            return static_cast<int>(card->outputChannelAt(index.at(0) - 1).ssm);
        }
        case kOutputCardOutChannelSsmControlMode: {
            int type = var;
            switch (type) {
                case ssmModeAUTO:
                case ssmModeMANUAL:
                case ssmModeOFF:
                    card->setSsmControlMode(index.at(0) - 1, static_cast<SSU::SsmMode>(type));
                    break;
                default:
                    errorCode = ErrorCodes::BadValue;
                    break;
            }
            return static_cast<int>(card->outputChannelAt(index.at(0) - 1).ssmControlMode);
        }
        default: break;
    }

    return NoSuchInstance();
}
