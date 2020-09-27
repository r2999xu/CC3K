//
// Created by JamesXu on 2020/8/3.
//

#include "map.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;

void vector_substract(vector<Posn> &, const vector<Posn>);

bool is_enemy(char c) {
    vector<char> v = {'H','W','E','O','M','L','D'};
    if (find(v.begin(), v.end(), c) != v.end()) {
        return true;
    }
    return false;
}

Map::Map(int level,string player_race) : level{level},upstair{Posn(-2,-2)}{
    freeze = false;
    player = make_shared<Player>(player_race,Posn(-1,-1),this);
    for (int i = 0; i < 5; i++)
    {
        chambers.emplace_back(make_shared<Chamber>(Chamber(i)));
    }
    for (int i=0;i<25;i++) {
        vector<char> v;
        layout.emplace_back(v);
        for (int j=0;j<80;j++) {
            v.emplace_back(' ');
        }
    }
}

void Map::spawn_read(ifstream & file) {
    //we first read everything into the layout
    int top_line = (level-1) * 25;
    int bottom_line = top_line + 24;
    string s;
    for(int i=0;i<125;i++) {
        getline(file,s);
        if (i >= top_line && i <= bottom_line) { // we only care about a specific map when reading the file,make sure we get into the map we need
            int layout_index = i-top_line; //get the index in our current map layout (i-th row)
            for(int j=0;j<80;j++) {
                layout[layout_index].emplace_back(s[j]);//get j-th col in i-th row
                if(s[j] == '+') {
                    doors.emplace_back(Posn(i,j));
                }
                if(s[j] == '#') {
                    paths.emplace_back(Posn(i,j));
                }
                if(s[j] == '\\') {
                    upstair = Posn(i,j);
                }
            }
        }
        if (i > bottom_line) { // we go past the last line belonging to our map
            break;
        }
    }
    //we finish reading things into layout
    //now start classifying things into its class vector

    for (int i=0;i<25;i++) {
        for(int j=0;j<80;j++) {
            char element = layout[i][j];
            // here since everything has a default value(attack,health,etc) we only need a posn to construct an object
            if (is_enemy(element)) {
                shared_ptr<Enemy> e1;
                if (element == 'H') {
                    auto e1 = make_shared<Human>(Posn(i, j),this);
                    enemies.emplace_back(e1);
                }
                else if (element == 'W') {
                    auto e1 = make_shared<Dwarf>(Posn(i,j),this);
                    enemies.emplace_back(e1);
                }
                else if (element == 'E') {
                    auto e1 = make_shared<Elf>(Posn(i,j),this);
                    enemies.emplace_back(e1);
                }
                else if (element == 'O') {
                    auto e1 = make_shared<Orc>(Posn(i,j),this);
                    enemies.emplace_back(e1);
                }
                else if (element == 'M') {
                    auto e1 = make_shared<Merchant>(Posn(i,j),this);
                    enemies.emplace_back(e1);
                }
                else if (element == 'L') {
                    auto e1 = make_shared<Halfling>(Posn(i,j),this);
                    enemies.emplace_back(e1);
                }
                else if (element == 'D') {
                    auto e1 = make_shared<Dragon>(Posn(i,j),this);
                    enemies.emplace_back(e1);
                }
            } else if(element >= '0' && element <= '5') {
                string item_name = potion_map[element];
                auto po1 = make_shared<Potion>(item_name,Posn(i,j));
                potions.emplace_back(po1);
            } else if(element >= '6' && element <= '9')  {
                string item_name = gold_map[element];
                if(element != '9') {
                    auto g1 = make_shared<Gold>(item_name, Posn(i, j));
                    golds.emplace_back(g1);
                }else {
                    auto e1 = make_shared<Dragon_hoard>(Posn(i, j),this);
                    enemies.emplace_back(e1);
                }
            } else if(element == '@') {
                auto pl1 = make_shared<Player>(player->getRace(),Posn(i,j),this);
                player = pl1;
            }
        }
    }
    //assign dragon pointer to dragon hoard, and assign dragon hoard pointer to dragon hoard
    for(auto e : enemies) {
        if(e->getRace() == "Dragon") {
            for(auto e1 : enemies) {
                if(e1->getRace() == "Dragon_hoard" && abs(e1->getPosn().getX()-e->getPosn().getX()) <= 1 && abs(e1->getPosn().getY()-e->getPosn().getY()) <= 1) {
                    Enemy *f = e.get();
                    Dragon *d = dynamic_cast<Dragon *>(f);
                    Enemy *g = e1.get();
                    Dragon_hoard *h = dynamic_cast<Dragon_hoard *>(g);
                    d->ptr_to_Dargon_hoard = h;
                }
            }
        }
        if(e->getRace() == "Dragon_hoard") {
            for(auto e1 : enemies) {
                if(e1->getRace() == "Dragon" && abs(e1->getPosn().getX()-e->getPosn().getX()) <= 1 && abs(e1->getPosn().getY()-e->getPosn().getY()) <= 1) {
                    Enemy *f = e.get();
                    Dragon_hoard *d = dynamic_cast<Dragon_hoard *>(f);
                    Enemy *g = e1.get();
                    Dragon *h = dynamic_cast<Dragon *>(g);
                    d->ptr_to_Dragon = h;
                }
            }
        }
    }
}
void Map::random_spawn()
{
    int s = time(NULL);
    srand(time(NULL));
    ifstream f = ifstream("cc3kdefaultfloor.txt");
    if (f.is_open())
    {
        read_layout(f);
    }
    else
    {
        throw "Default Layout File Not Found";
    }
    vector<Posn> occupied;
    vector<Posn> available_chamber_spots;
    vector<Posn> dragons; // store locations of dragons
    vector<Dragon_hoard *> dragon_hoards;
    int random_chamber = rand() % 5;
    int size;
    int dragon_num = 0;
    Posn random_posn{0, 0};
    // Randomize player's location     
    random_chamber = rand() % 5; // randomly put player in one of the 5 chambers
    srand(++s); // reset seed
    int player_chamber = random_chamber; // store player's chamber to avoid spawning stairway in the same chamber
    available_chamber_spots = chambers[random_chamber]->inside;
    vector_substract(available_chamber_spots, occupied);
    size = available_chamber_spots.size();
    random_posn = available_chamber_spots[rand() % size];
    auto pl1 = make_shared<Player>(player->getRace(),random_posn,this);
    player = pl1;
    occupied.emplace_back(random_posn);   
    update();
    // Randomize stair's location
    srand(++s);
    while (random_chamber == player_chamber) // randomly put stairway in one of the other 4 chambers;
    {
        random_chamber = rand() % 5;
        srand(++s);
    }
    available_chamber_spots = chambers[random_chamber]->inside;
    vector_substract(available_chamber_spots, occupied);
    size = available_chamber_spots.size();
    random_posn = available_chamber_spots[rand() % size];
    srand(++s);
    layout.at(random_posn.x).at(random_posn.y) = '\\'; // spawn stairway
    occupied.emplace_back(random_posn);
    // Randomly spawn potions
    for (int i = 0; i < 10; i++)
    {
        random_chamber = rand() % 5;
        srand(++s);
        available_chamber_spots = chambers[random_chamber]->inside;
        vector_substract(available_chamber_spots, occupied);
        size = available_chamber_spots.size();
        random_posn = available_chamber_spots[rand() % size];
        string item_name = potion_map['0' + rand() % 6];
        srand(++s);
        auto potion = make_shared<Potion>(item_name, random_posn);
        potions.emplace_back(potion);
        occupied.emplace_back(random_posn);
    }
    update();
    // Randomly spawn gold piles
    int gold_type;
    for (int i = 0; i < 10; i++)
    {
        random_chamber = rand() % 5;
        srand(++s);
        available_chamber_spots = chambers[random_chamber]->inside;
        vector_substract(available_chamber_spots, occupied);
        size = available_chamber_spots.size();
        random_posn = available_chamber_spots[rand() % size];
        srand(++s);
        string item_name;
        gold_type = rand() % 8;
        if (gold_type <= 4) {
            item_name = gold_map['6'];
        } else if (gold_type == 5) {
            item_name = gold_map['9'];
            dragon_num++;
            auto hoard = make_shared<Dragon_hoard>(random_posn, this);
            dragon_hoards.emplace_back(&*hoard);
            enemies.emplace_back(hoard);
            // mark the corresponding dragon's location as occupied
            for (int i = 1; i >= -1; i--) {
                for (int j = -1; j <= 1; j++) {
                    int x = random_posn.x + i;
                    int y = random_posn.y + j;
                    if (layout.at(x).at(y) == '.' && i != 0 && j != 0) {
                        dragons.emplace_back(Posn(x,y));
                        occupied.emplace_back(Posn{x,y});
                        break;
                    }
                }
            } // for
        } else {
            item_name = gold_map['7'];
        } // if
        srand(++s);
        auto gold = make_shared<Gold>(item_name, random_posn);
        golds.emplace_back(gold);
        occupied.emplace_back(random_posn);
        update();
    } //for
    // Randomly spawn enemies
    int enemy_index;
    for (int i = 0; i < 20; i++)
    {
        enemy_index = rand() % 6;
        srand(++s);
        random_chamber = rand() % 5;
        srand(++s);
        available_chamber_spots = chambers[random_chamber]->inside;
        vector_substract(available_chamber_spots, occupied);
        size = available_chamber_spots.size();
        random_posn = available_chamber_spots[rand() % size];
        srand(++s);
        shared_ptr<Enemy> enemy;
        if (enemy_index == 0)
        {
            enemy = make_shared<Human>(random_posn, this);
            enemies.emplace_back(enemy);
        }
        else if (enemy_index == 1)
        {
            enemy = make_shared<Dwarf>(random_posn, this);
            enemies.emplace_back(enemy);
        }
        else if (enemy_index == 2)
        {
            enemy = make_shared<Elf>(random_posn, this);
            enemies.emplace_back(enemy);
        }
        else if (enemy_index == 3)
        {
            enemy = make_shared<Orc>(random_posn, this);
            enemies.emplace_back(enemy);
        }
        else if (enemy_index == 4)
        {
            enemy = make_shared<Merchant>(random_posn, this);
            enemies.emplace_back(enemy);
        }
        else
        {
            enemy = make_shared<Halfling>(random_posn, this);
            enemies.emplace_back(enemy);
        }
        occupied.emplace_back(random_posn);
        update();
    } //for
    // Spawn dragons 1 block around dragon hoards
    for (int i = 0; i < dragon_num; i++)
    {
        auto enemy = make_shared<Dragon>(dragons[i], this);
        enemy->ptr_to_Dargon_hoard = dragon_hoards[i];
        dragon_hoards[i]->ptr_to_Dragon = &*enemy;
        enemies.emplace_back(enemy);
        update();
    }
}

void Map::update() {
    vector<Posn> all_updated_posns; // stores the current version of all changeable elements
    vector<char> all_changeable_element = {'@','H','W','E','O','M','L','D','P','G'}; // stores symbols of all changeable elements
    for (auto e : enemies) {
        Posn pos1 = e->getPosn();
        all_updated_posns.emplace_back(pos1);
        string race = e->getRace();
        char r = enemy_map[race];
        layout.at(pos1.getX()).at(pos1.getY()) = r;
    }
    for (auto p : potions) {
        Posn pos1 = p->getPosn();
        all_updated_posns.emplace_back(pos1);
        layout.at(pos1.getX()).at(pos1.getY()) = 'P';
    }
    for (auto g : golds) {
        Posn pos1 = g->getPosn();
        all_updated_posns.emplace_back(pos1);
        layout.at(pos1.getX()).at(pos1.getY()) = 'G';
    }
    Posn player_pos = player->getPosn();
    all_updated_posns.emplace_back(player_pos);
    layout.at(player_pos.getX()).at(player_pos.getY()) = '@';
    for(int i=0;i<25;i++) {
        for(int j=0;j<80;j++) {
            //if we remove something,its posn won't be in all updated posns, but its symbol is still in layout,we set all newest version of changeable element symbol in layout,
            //but the old version of changeable element symbol still exists in layout, we need to test if that symbol also has a posn in our all_updated_posns, if not,
            //then it's already removed, thus we change it to a dot.
            if (find(all_changeable_element.begin(), all_changeable_element.end(), layout[i][j]) != all_changeable_element.end()) {
                if (find(all_updated_posns.begin(), all_updated_posns.end(), Posn(i,j)) == all_updated_posns.end()) {
                    if(find(doors.begin(), doors.end(), Posn(i,j)) != doors.end()) {
                        layout[i][j] = '+';
                    } else if(find(paths.begin(), paths.end(), Posn(i,j)) != paths.end()) {
                        layout[i][j] = '#';
                    } else {
                        layout[i][j] = '.';
                    }
                }
            }
        }
    }
}

void Map::print() {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
             cout<<layout[i][j];
            if (j == 79) {
                cout << endl;
            }
        }
    }
}

void Map::read_layout(ifstream &file)
{
    string s;
    for (int i = 0; i < 25; i++)
    {
        getline(file, s);
        for (int j = 0; j < 80; j++)
        {
            layout[i].emplace_back(s[j]); //get j-th col in i-th row
            if(s[j] == '+') {
                    doors.emplace_back(Posn(i,j));
                }
                if(s[j] == '#') {
                    paths.emplace_back(Posn(i,j));
                }
                if(s[j] == '\\') {
                    upstair = Posn(i,j);
            }
        }
    }
}

void vector_substract(vector<Posn> & vec1, const vector<Posn> vec2) // removes vectors in the intersection of vec1 and vec2 from vec1
{
    for (auto i = vec1.begin(); i != vec1.end(); i++)
    {
        for (auto j : vec2)
        {
            if (i->x == j.x && i->y == j.y)
            {
                vec1.erase(i);
                --i;
            }
        }
    }
}