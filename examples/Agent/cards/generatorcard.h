#ifndef GENERATOR_H
#define GENERATOR_H

#include <QVector>
#include "card.h"

class GeneratorCard : public Card
{
public:
    GeneratorCard();

    SSU::GeneratorMode getGeneratorMode() const;
    void setGeneratorMode(const SSU::GeneratorMode &value);

    SSU::GeneratorState getGeneratorState() const;
    void setGeneratorState(const SSU::GeneratorState &value);

    int getManualChannel() const;
    void setManualChannel(int value);

    int getActiveChannel() const;
    void setActiveChannel(int value);

    struct InputChannel {
        bool ban = false;
        int priority = 0;
        int currentPriority = 0;
    };

    int numberOfInputs() const;
    InputChannel channelAt(int ind) const;

    void setPriority(int ind, int value);
    void setBan(int ind, bool value);
    void setCurrentPriority(int ind, int value);

private:
    int activeChannel;
    SSU::GeneratorMode generatorMode;
    SSU::GeneratorState generatorState;
    int manualChannel;

    QVector <InputChannel> channels;
};

#endif // GENERATOR_H
