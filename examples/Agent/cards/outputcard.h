#ifndef OUTPUTCARD_H
#define OUTPUTCARD_H

#include <QVector>
#include "card.h"

class OutputCard : public Card
{
public:
    OutputCard();

    struct InputChannel {
        int priority = 0;
        int currentPriority = 0;
        bool ban = false;
    };

    struct OutputChannel {
        SSU::SignalType type = SSU::SignalTypeOff;
        SSU::SsmMode ssmControlMode = SSU::ssmModeOFF;    // режим управления ssm битами
        SSU::SSM ssm = SSU::NONE;
    };

    InputChannel inputChannelAt(int ind) const;
    OutputChannel outputChannelAt(int ind) const;

    void setType(int ind, SSU::SignalType value);
    void setSsm(int ind, SSU::SSM value);
    void setSsmControlMode(int ind, SSU::SsmMode value);


    int getActiveChannel() const;
    void setActiveChannel(int value);

    int getBackupChannel() const;
    void setBackupChannel(int value);

    bool getSynchronization() const;
    void setSynchronization(bool value);

    SSU::CardMode getMode() const;
    void setMode(const SSU::CardMode &value);

private:
    SSU::CardMode mode;

    int activeChannel;
    int backupChannel;

    bool synchronization;

    QVector <InputChannel> inputChannels;
    QVector <OutputChannel> outputChannels;
};

#endif // OUTPUTCARD_H
