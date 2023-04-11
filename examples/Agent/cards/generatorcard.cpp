#include "generatorcard.h"

GeneratorCard::GeneratorCard() : Card(SSU::Generator), activeChannel(0), generatorMode(SSU::GModeAuto), generatorState(SSU::Warmup), manualChannel(0)
{
    channels = QVector <InputChannel>(NUMBER_OF_GENERATOR_CARD_INPUTS);
}

SSU::GeneratorMode GeneratorCard::getGeneratorMode() const
{
    return generatorMode;
}

void GeneratorCard::setGeneratorMode(const SSU::GeneratorMode &value)
{
    generatorMode = value;
}

SSU::GeneratorState GeneratorCard::getGeneratorState() const
{
    return generatorState;
}

void GeneratorCard::setGeneratorState(const SSU::GeneratorState &value)
{
    generatorState = value;
}

int GeneratorCard::getManualChannel() const
{
    return manualChannel;
}

void GeneratorCard::setManualChannel(int value)
{
    manualChannel = value;
}

int GeneratorCard::getActiveChannel() const
{
    return activeChannel;
}

void GeneratorCard::setActiveChannel(int value)
{
    activeChannel = value;
}

int GeneratorCard::numberOfInputs() const
{
    return channels.size();
}

GeneratorCard::InputChannel GeneratorCard::channelAt(int ind) const
{
    return channels.at(ind);
}

void GeneratorCard::setPriority(int ind, int value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].priority = value;
}

void GeneratorCard::setBan(int ind, bool value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].ban = value;
}

void GeneratorCard::setCurrentPriority(int ind, int value)
{
    if(ind < 0 || ind >= channels.size()) return;
    channels[ind].currentPriority = value;
}



