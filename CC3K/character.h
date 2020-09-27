//
// Created by JamesXu on 2020/8/4.
//

#ifndef ACTUAL_CHARACTER_H
#define ACTUAL_CHARACTER_H

#include <vector>
#include <memory>
#include <string>

#include "posn.h"
#include "gold.h"

using namespace std;

class Map;

/*
enum race {Shade,Drow,Vampire,Troll,Goblin,Human,Dwarf,Elf,Orc,Merchant,Halfling,Dragon};
enum player_race {Shade,Drow,Vampire,Troll,Goblin};
enum enermy_race {Human,Dwarf,Elf,Orc,Merchant,Halfling,Dragon};
*/
class Player;

class Character{

public:
    Map * whole_Map;
    int hp;
    int attack_num;
//watch out for bug here!!!
    string race;
    int defence;
    Posn pos;
    vector<char> surround;

    Character();
    virtual void die() = 0;
    virtual void attack(string) = 0 ;
    virtual void move(string) = 0 ;
    void generate_surround();
    Posn getPosn();
    string getRace();

};


#endif //ACTUAL_CHARACTER_H
