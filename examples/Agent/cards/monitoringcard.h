#ifndef MONITORINGCARD_H
#define MONITORINGCARD_H

#include "card.h"

class MonitoringCard : public Card
{
public:
    MonitoringCard();

    QString getName() const;
    void setName(const QString &value);

    QString getIp() const;
    void setIp(const QString &value);

    QString getMask() const;
    void setMask(const QString &value);

    quint64 getPort() const;
    void setPort(const quint64 &value);

    bool getDeviceThresholdsEnabled() const;
    void setDeviceThresholdsEnabled(bool value);

    struct PowerCard {
        bool power;
        bool fuse;
    };

    PowerCard getPowerCard1() const;
    void setPowerCard1(const PowerCard &value);

    PowerCard getPowerCard2() const;
    void setPowerCard2(const PowerCard &value);

private:
    QString name;
    QString ip;
    QString mask;
    quint64 port;

    PowerCard powerCard1;
    PowerCard powerCard2;


    bool deviceThresholdsEnabled;
};

#endif // MONITORINGCARD_H
