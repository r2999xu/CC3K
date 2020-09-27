//
// Created by JamesXu on 2020/8/3.
//

#ifndef CC3K_MAP_H
#define CC3K_MAP_H

#include <vector>
#include <memory>
#include <map>
#include "posn.h"
#include "enemy.h"
#include "player.h"
#include "chamber.h"
#include <ctime>
using namespace std;

class Map {
public:
    bool freeze;
    int level;
    int seed;
    vector<vector<char>>layout;
    vector<shared_ptr<Chamber>> chambers;
    vector<shared_ptr<Enemy>> enemies;
    vector<shared_ptr<Potion>> potions;
    vector<shared_ptr<Gold>> golds;
    vector<Posn> paths;
    vector<Posn> doors;
    Posn upstair;
    shared_ptr<Player> player;
    map<string,char> enemy_map = {{"Human",'H'},{"Dwarf",'W'},{"Elf",'E'},{"Orc",'O'},{"Merchant",'M'},{"Halfling",'L'},{"Dragon",'D'},{"Dragon_hoard",'G'}};
    map<char,string> potion_map = {{'0',"RH"}, {'1',"BA"}, {'2',"BD"}, {'3',"PH"}, {'4',"WA"}, {'5',"WD"}};
    map<char,string> gold_map = {{'6',"normal gold pile"}, {'7',"small hoard"}, {'8',"merchant hoard"}, {'9',"dragon hoard"}};
    Map(int level,string player_race);
    void read_layout(ifstream & file);
    void spawn_read(ifstream & file);//read all information from the file into layout and classify them into their own class vector
    void random_spawn();
    void update();//update all information in layout
    void print();//print the current version of layout in the map
};


#endif //CC3K_MAP_H
