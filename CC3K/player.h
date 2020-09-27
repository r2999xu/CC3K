//
// Created by JamesXu on 2020/8/4.
//

#ifndef ACTUAL_PLAYER_H
#define ACTUAL_PLAYER_H


#include "character.h"
#include <map>
#include <vector>
#include <memory>
#include <string>
#include "potion.h"
#include "gold.h"

using namespace std;

class Player :public Character {

public:
    int level;
    //in base data we have : attack_num ,defence
    map<string,int> base_data;
    //in base data we have : attack_num ,defence    
    map<string,int> buffed_data;
    
    vector<shared_ptr<Potion>> potions_list;
    int Gold_num;

    //this is what system output for player it's a string 
    string str_out;

    Player(string race,Posn pos,Map * map);

    void use(string str) ;

    void move(string str) ;

    void attack(string) ;

    void die() ;

    void analyse_buff();



};



#endif //ACTUAL_PLAYER_H
