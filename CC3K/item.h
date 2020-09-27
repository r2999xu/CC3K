//
// Created by JamesXu on 2020/8/4.
//

#ifndef CC3K_ITEM_H
#define CC3K_ITEM_H

#include "posn.h"
#include <string>
using namespace std;

class Item{
    string item_name;
    Posn item_posn;
public:
    Item(const string item_name,Posn item_posn);
    string getName();
    Posn getPosn();
};


#endif //CC3K_ITEM_H
