#include "outputcard.h"

OutputCard::OutputCard() : Card(SSU::OutputH)
{
    inputChannels = QVector <InputChannel>(NUMBER_OF_OUTPUT_CARD_INPUTS);
    outputChannels = QVector <OutputChannel>(NUMBER_OF_OUTPUT_CARD_OUTPUTS);
}

OutputCard::InputChannel OutputCard::inputChannelAt(int ind) const
{
    return inputChannels.at(ind);
}

OutputCard::OutputChannel OutputCard::outputChannelAt(int ind) const
{
    return outputChannels.at(ind);
}

void OutputCard::setType(int ind, SSU::SignalType value)
{
    if(ind < 0 || ind >= outputChannels.size()) return;
    outputChannels[ind].type = value;
}

void OutputCard::setSsm(int ind, SSU::SSM value)
{
    if(ind < 0 || ind >= outputChannels.size()) return;
    outputChannels[ind].ssm = value;
}

void OutputCard::setSsmControlMode(int ind, SSU::SsmMode value)
{
    if(ind < 0 || ind >= outputChannels.size()) return;
    outputChannels[ind].ssmControlMode = value;
}

int OutputCard::getActiveChannel() const
{
    return activeChannel;
}

void OutputCard::setActiveChannel(int value)
{
    activeChannel = value;
}

int OutputCard::getBackupChannel() const
{
    return backupChannel;
}

void OutputCard::setBackupChannel(int value)
{
    backupChannel = value;
}

bool OutputCard::getSynchronization() const
{
    return synchronization;
}

void OutputCard::setSynchronization(bool value)
{
    synchronization = value;
}

SSU::CardMode OutputCard::getMode() const
{
    return mode;
}

void OutputCard::setMode(const SSU::CardMode &value)
{
    mode = value;
}
