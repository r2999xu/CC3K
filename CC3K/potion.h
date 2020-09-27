//
// Created by JamesXu on 2020/8/4.
//

#ifndef CC3K_POTION_H
#define CC3K_POTION_H

#include "item.h"

class Potion: public Item {
public:
    Potion(const string item_name,Posn item_posn);
};


#endif //CC3K_POTION_H
