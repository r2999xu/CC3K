//
// Created by JamesXu on 2020/8/4.
//

#include "item.h"

Item::Item(const string item_name,Posn item_posn):item_name{item_name},item_posn{item_posn} {}

string Item::getName() {
    return item_name;
}

Posn Item::getPosn() {
    return item_posn;
}

