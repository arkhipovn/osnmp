#include "inputcard.h"

InputCard::InputCard() : Card(SSU::Input)
{
    channels = QVector <InputChannel>(NUMBER_OF_INPUT_CARD_INPUTS);
}

int InputCard::numberOfInputs() const
{
    return channels.size();
}

InputCard::InputChannel InputCard::channelAt(int ind) const
{
    return channels.at(ind);
}

void InputCard::setBan(int ind, bool value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].ban = value;
}

void InputCard::setState(int ind, int value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].state = value;
}

void InputCard::setType(int ind, SSU::SignalType value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].type = value;
}

void InputCard::setStatus(int ind, SSU::StatusThreshold value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].status = value;
}

void InputCard::setStatusThreshold(int ind, SSU::StatusThreshold value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].statusThreshold = value;
}

void InputCard::setSsm(int ind, SSU::SSM value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].ssm = value;
}

void InputCard::setSsmThreshold(int ind, SSU::SSM value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].ssmThreshold = value;
}

void InputCard::setWaitToRestoreTime(int ind, int value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].waitToRestoreTime = value;
}
