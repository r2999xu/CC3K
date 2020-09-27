//
// Created by JamesXu on 2020/8/4.
//
#include <iostream>
#include "posn.h"

Posn::Posn(int x, int y){
    this->x = x;
    this->y = y;
}

int Posn::getX() {
    return this->x;
}
int Posn::getY() {
    return this->y;
}

bool Posn::operator==(Posn other) {
    if(getX() == other.getX() && getY() == other.getY()) {
        return true;
    }
    return false;
} 
