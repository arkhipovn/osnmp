#ifndef INPUTCARD_H
#define INPUTCARD_H

#include <QVector>
#include "card.h"

class InputCard : public Card
{
public:
    InputCard();

    struct InputChannel {
        bool ban = false;
        int state = 0;

        SSU::SignalType type = SSU::SignalTypeOff;

        SSU::StatusThreshold status = SSU::StatusThresholdHigh;
        SSU::StatusThreshold statusThreshold = SSU::StatusThresholdNone;
        SSU::SSM ssm = SSU::NONE;
        SSU::SSM ssmThreshold = SSU::PRC;   // SSU::SSMNone;

        int waitToRestoreTime = 1;  // мин
    };

    int numberOfInputs() const;
    InputChannel channelAt(int ind) const;


    void setBan(int ind, bool value);
    void setState(int ind, int value);
    void setType(int ind, SSU::SignalType value);
    void setStatus(int ind, SSU::StatusThreshold value);
    void setStatusThreshold(int ind, SSU::StatusThreshold value);
    void setSsm(int ind, SSU::SSM value);
    void setSsmThreshold(int ind, SSU::SSM value);
    void setWaitToRestoreTime(int ind, int value);

private:
    QVector <InputChannel> channels;
};

#endif // INPUTCARD_H
