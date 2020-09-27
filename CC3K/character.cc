//
// Created by JamesXu on 2020/8/4.
//

#include "character.h"
#include <string>
#include "map.h"
using namespace std;

Character::Character():whole_Map{nullptr},hp{0},attack_num{0},race{""},defence{0},pos{Posn(-1,-1)}{}

Posn Character::getPosn(){
    return this->pos;
}

string Character::getRace(){
    return this->race;

}

void Character::generate_surround(){

    //if you want debug and print each surround please change this debug to true
    bool debug = false;
    //

    int up_down = this->getPosn().x;
    int left_right = this->getPosn().y;

    // std::cout <<"Character :"<<this->race<<" pos is :"<<up_down<<" "<<left_right<<std::endl;
    char my_race = this->whole_Map->layout[up_down][left_right];

    char d1 = this->whole_Map->layout[up_down + 1][left_right - 1];
    char d2 = this->whole_Map->layout[up_down + 1][left_right];
    char d3 = this->whole_Map->layout[up_down + 1][left_right + 1 ];
    char d4 = this->whole_Map->layout[up_down ][left_right - 1];
    char d5 = this->whole_Map->layout[up_down ][left_right + 1 ];
    char d6 = this->whole_Map->layout[up_down - 1][left_right - 1];
    char d7 = this->whole_Map->layout[up_down - 1][left_right  ];
    char d8 = this->whole_Map->layout[up_down - 1][left_right + 1 ];

if(debug == true){
  std::cout << "surround :"<< std::endl;
  std::cout << d6 <<" " <<d7 <<" " << d8 <<" "<< std::endl;
  std::cout << d4 <<" " <<my_race <<" " << d5 <<" "<< std::endl;
  std::cout << d1 <<" " <<d2 <<" " << d3 <<" "<< std::endl;
  std::cout << std::endl;
}
this->surround.clear();

this->surround.emplace_back(d1);
this->surround.emplace_back(d2);
this->surround.emplace_back(d3);
this->surround.emplace_back(d4);
this->surround.emplace_back(d5);
this->surround.emplace_back(d6);
this->surround.emplace_back(d7);
this->surround.emplace_back(d8);


}

