#ifndef INVENTORY_H
#define INVENTORY_H

#include <QMap>
#include "cards/card.h"

class Inventory
{
public:
    using CardMap = QMap <int, Card*>;

    Inventory();
    ~Inventory();

    void append(int slot, Card* card);

    using iterator = Inventory::CardMap::iterator;

    bool isEmpty() const { return cards.isEmpty(); }

    iterator upperBound(int slot) { return cards.upperBound(slot); }
    iterator find(int slot) { return cards.find(slot); }
    iterator begin() { return cards.begin(); }
    iterator end() { return cards.end(); }

    int size() const { return cards.size(); }

private:
    CardMap cards;
};

#endif // INVENTORY_H
