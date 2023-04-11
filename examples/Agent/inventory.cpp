#include "inventory.h"

Inventory::Inventory()
{

}

Inventory::~Inventory()
{
    auto it = cards.begin();
    while(it != cards.end()) {
        delete it.value();
        ++it;
    }
}

void Inventory::append(int slot, Card *card)
{
    cards.insert(slot, card);
}
