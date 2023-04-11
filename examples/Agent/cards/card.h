#ifndef CARD_H
#define CARD_H

#include <QString>
#include "ssu.h"

class Card {
public:
    Card(SSU::CardType type) : type(type) {}
    virtual ~Card() = default;

    SSU::CardType getType() const { return type; }

    QString getSerialNumber() const { return serialNumber; }
    void setSerialNumber(const QString &value) { serialNumber = value; }

    QString getSoftware() const { return software; }
    void setSoftware(const QString &value) { software = value; }

    QString getHardware() const { return hardware; }
    void setHardware(const QString &value) { hardware = value; }

protected:
    SSU::CardType type;
    QString serialNumber;
    QString software;
    QString hardware;
};

#endif // CARD_H
