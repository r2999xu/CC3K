//
// Created by JamesXu on 2020/8/4.
//

#ifndef ACTUAL_POSN_H
#define ACTUAL_POSN_H


class Posn{

public:
    Posn(int x,int y);
    int x;
    int y;
    int getX();
    int getY();
    bool operator==(Posn other);
};

#endif //ACTUAL_POSN_H
