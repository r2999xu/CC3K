//
// Created by JamesXu on 2020/8/4.
//

#ifndef ACTUAL_ENEMY_H
#define ACTUAL_ENEMY_H


#include <iostream>

#include "character.h"
#include "gold.h"
#include "player.h"

#include <map>
#include <vector>
#include <memory>
#include <string>
using namespace std;



class Enemy :public Character {

public:
    bool attacked;

    void move(string) override;

    virtual void attack(string) override;

    virtual void die() override;


};

class Human :public Enemy{

public:
    Human(Posn,Map *);

//not implement yet!
    void die() override;

};

class Dwarf :public Enemy{

public:
    Dwarf(Posn,Map *);

//not implement yet!
    void attack(string) override;

};

class Elf :public Enemy{

public:
    Elf(Posn,Map *);

//not implement yet!
    void attack(string) override;

};

class Orc :public Enemy{

public:
    Orc(Posn,Map *);

//not implement yet!
    void attack(string) override;

};

class Merchant :public Enemy{

public:
    Merchant(Posn,Map *);

//not implement yet!
//we need surroud here!
    void attack(string) override;
    void die() override;

};


class Halfling :public Enemy{

public:
    Halfling(Posn,Map *);


};

class Dragon_hoard;

class Dragon :public Enemy{

public:

    Dragon_hoard * ptr_to_Dargon_hoard;
    Dragon(Posn,Map *);

    void attack(string) override;
//not implement yet!
//we need surroud here!
    void die() override;

};



class Dragon_hoard : public Enemy{

//if attack is 666 it's pickable
//if attack is 0 we cant pick
public:


    Dragon * ptr_to_Dragon;
    Dragon_hoard(Posn,Map *);

    //not implement yet!
    void attack(string) override;



};



#endif //ACTUAL_ENEMY_H
