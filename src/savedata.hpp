#include "card.hpp"
#include "stdint.h"

#ifndef SAVEDATA_H
#define SAVEDATA_H

union SaveData
{
public:
    uint8_t value;
    Card card;

    SaveData() { value = 0; }
};

#endif
