#include <iostream>
#include "map.h"
#include "character.h"
#include <fstream>
#include <string>
using namespace std;

map<string,string> player_race_map = {{"s","Shade"}, {"d","Drow"}, {"v","Vampire"}, {"g","Goblin"}, {"t","Troll"}};

bool is_change_character_cmd(string s) {
    if(s == "s" || s == "d" || s == "v" || s == "g" || s == "t") {
        return true;
    }
    return false;
}

void player_move_helper(string str,Map * map){
    if(str == "no"){
        map->player->move("no");
    }else if(str == "so"){
        map->player->move("so");
    }else if(str == "ea"){
        map->player->move("ea");
    }else if(str == "we"){
        map->player->move("we");
    }else if(str == "ne"){
        map->player->move("ne");
    }else if(str == "nw"){
        map->player->move("nw");
    }else if(str == "se"){
        map->player->move("se");
    }else if(str == "sw"){
        map->player->move("sw");
    }

}

int main(int argc,char* argv[]) {
    srand(time(NULL));
    if (argc == 2) {
        bool changeable = true;
        string file_name = argv[1];
        ifstream map_file{file_name};
        Map main_map = Map(1, "Shade");
        main_map.spawn_read(map_file);
        main_map.update();
        main_map.print();
        std::cout << "Race: " << main_map.player->race << " Gold: " << main_map.player->Gold_num <<" Floor: "<<main_map.level<< std::endl;
        std::cout << "HP: " << main_map.player->hp << std::endl;
        std::cout << "Atk: " << main_map.player->buffed_data["attack_num"] << std::endl;
        std::cout << "Def: " << main_map.player->buffed_data["defence"] << std::endl;
        std::cout << "Action: " << main_map.player->str_out << std::endl;
        while (true) {
            std::string str;
            std::cout << std::endl;
            std::cout << "enter your action: " << std::endl;

            std::cin >> str;


            std::cout << "you entered :" << str << std::endl;
            if (std::cin.fail()) {
                std::cout << "invalid input " << std::endl;
                break;
            }

            if (str == "r") {
                cout << "you restart the game" << endl;
                ifstream map_file{file_name};
                main_map = Map(1, "Shade");
                main_map.spawn_read(map_file);
                main_map.update();
                changeable = true;
            }

            if (is_change_character_cmd(str) && changeable) {
                ifstream map_file{file_name};
                main_map = Map(1, player_race_map[str]);
                main_map.spawn_read(map_file);
                main_map.update();
                changeable = false;
                cout<<"you changed character into ";
                cout<<player_race_map[str]<<endl;
            }

            if (str == "f") {
                if (main_map.freeze == true) {
                    main_map.freeze = false;
                    main_map.player->str_out += "enemy now can move";
                } else if (main_map.freeze == false) {
                    main_map.freeze = true;
                    main_map.player->str_out += "enemy now cant move";
                }
            }
            if (main_map.player->getRace() == "Troll") {
                main_map.player->str_out += " Troll will +5hp each turn ";
                if (main_map.player->hp + 5 > 120) {
                    main_map.player->hp = 120;
                    main_map.player->str_out += " You become FULL HP (cant over MAX HP)  ";
                } else {
                    main_map.player->hp += 5;

                }
            }

            if (str == "no" || str == "so" || str == "ea" || str == "we" || str == "ne" || str == "nw" || str == "se" ||
                str == "sw") {
                player_move_helper(str, &main_map);
                if (main_map.player->level != main_map.level) {
                    if(main_map.player->level == 5) {
                        cout << "Congratulations! You finished the game! Your score is ";
                        if(main_map.player->race != "Shade") {
                            cout << main_map.player->Gold_num << "." << endl;
                        }else {
                            int score = main_map.player->Gold_num * 1.5;
                            cout << score << "." <<endl;
                        }
                        if (main_map.player->hp == 0) {
                            cout << "Ohhh! You finished the game in GHOST MODE, since you died already XD!" << endl;
                        }
                        cout <<"Do you want to restart or quit (r/q)";
                        continue;
                    }
                    int Gold_num = main_map.player->Gold_num;
                    string str_out = main_map.player->str_out;
                    int health = main_map.player->hp;
                    main_map = Map(main_map.player->level, main_map.player->race);
                    ifstream map_file1{file_name};
                    main_map.spawn_read(map_file1);
                    main_map.player->Gold_num = Gold_num;
                    main_map.player->str_out = str_out;
                    main_map.player->hp = health;
                    main_map.update();
                }
            }

            if (str == "uno" || str == "uso" || str == "uea" || str == "uwe" || str == "une" || str == "unw" ||
                str == "use" || str == "usw") {
                main_map.player->use(str.substr(1));
            }
            if (str == "ano" || str == "aso" || str == "aea" || str == "awe" || str == "ane" || str == "anw" ||
                str == "ase" || str == "asw") {
                main_map.player->attack(str.substr(1));
            }

            if (str == "q") {
                std::cout << "quit!!!" << std::endl;
                break;
            }


            for (auto n :main_map.enemies) {
                //std::cout<<"ennemies names :"<< n->getRace()<<std::endl;
                // n->move("");
                n->attack("");
            }
            main_map.print();
            std::cout << "Race: " << main_map.player->race << " Gold: " << main_map.player->Gold_num << std::endl;
            std::cout << "HP: " << main_map.player->hp << std::endl;
            std::cout << "Atk: " << main_map.player->buffed_data["attack_num"] << std::endl;
            std::cout << "Def: " << main_map.player->buffed_data["defence"] << std::endl;
            std::cout << "Action: " << main_map.player->str_out << std::endl;
            if(main_map.player->hp == 0) {
                continue;
            }
            main_map.player->str_out = "";
        }

    }
    if (argc == 1) {
        bool changeable = true;
        Map main_map = Map(1, "Shade");
        main_map.random_spawn();
        main_map.update();
        main_map.print();
        while (true) {
            std::string str;
            std::cout << std::endl;
            std::cout << "enter your action: " << std::endl;

            std::cin >> str;


            std::cout << "you entered :" << str << std::endl;
            if (std::cin.fail()) {
                std::cout << "invalid input " << std::endl;
                break;
            }

            if (str == "r") {
                cout << "you restarted the game" << endl;
                main_map = Map(1, "Shade");
                main_map.random_spawn();
                main_map.update();
                changeable = true;
            }

            if (is_change_character_cmd(str) && changeable) {
                main_map = Map(1, player_race_map[str]);
                main_map.random_spawn();
                main_map.update();
                changeable = false;
                cout<<"you changed character info ";
                cout<<player_race_map[str]<<endl;
            }

            if (str == "f") {
                if (main_map.freeze == true) {
                    main_map.freeze = false;
                    main_map.player->str_out += "enemy now can move";
                } else if (main_map.freeze == false) {
                    main_map.freeze = true;
                    main_map.player->str_out += "enemy now cant move";
                }
            }
            if (main_map.player->getRace() == "Troll") {
                main_map.player->str_out += " Troll will +5hp each turn ";
                if (main_map.player->hp + 5 > 120) {
                    main_map.player->hp = 120;
                    main_map.player->str_out += " You become FULL HP (cant over MAX HP)  ";
                } else {
                    main_map.player->hp += 5;

                }
            }

            if (str == "no" || str == "so" || str == "ea" || str == "we" || str == "ne" || str == "nw" || str == "se" ||
                str == "sw") {
                player_move_helper(str, &main_map);
                if (main_map.player->level != main_map.level) {
                    if(main_map.player->level == 5) {
                        cout << "Congratulations! You finished the game! Your score is ";
                        if(main_map.player->race != "Shade") {
                            cout << main_map.player->Gold_num << "." << endl;
                        }else {
                            int score = main_map.player->Gold_num * 1.5;
                            cout << score << "." <<endl;
                        }
                        if (main_map.player->hp == 0) {
                            cout << "Ohhh! You finished the game in GHOST MODE, since you died already XD!" << endl;
                        }
                        cout <<"Do you want to restart or quit (r/q)";
                        continue;
                    }
                    int Gold_num = main_map.player->Gold_num;
                    string str_out = main_map.player->str_out;
                    int health = main_map.player->hp;
                    main_map = Map(main_map.player->level, main_map.player->race);
                    main_map.random_spawn();
                    main_map.player->Gold_num = Gold_num;
                    main_map.player->str_out = str_out;
                    main_map.player->hp = health;
                    main_map.update();
                }
            }

            if (str == "uno" || str == "uso" || str == "uea" || str == "uwe" || str == "une" || str == "unw" ||
                str == "use" || str == "usw") {
                main_map.player->use(str.substr(1));
            }
            if (str == "ano" || str == "aso" || str == "aea" || str == "awe" || str == "ane" || str == "anw" ||
                str == "ase" || str == "asw") {
                main_map.player->attack(str.substr(1));
            }

            if (str == "q") {
                std::cout << "quit!!!" << std::endl;
                break;
            }


            for (auto n :main_map.enemies) {
                //std::cout<<"ennemies names :"<< n->getRace()<<std::endl;
                // n->move("");
                n->attack("");
            }
            main_map.print();
            std::cout << "Race: " << main_map.player->race << " Gold: " << main_map.player->Gold_num <<" Floor: "<<main_map.level<< std::endl;
            std::cout << "HP: " << main_map.player->hp << std::endl;
            std::cout << "Atk: " << main_map.player->buffed_data["attack_num"] << std::endl;
            std::cout << "Def: " << main_map.player->buffed_data["defence"] << std::endl;
            std::cout << "Action: " << main_map.player->str_out << std::endl;
            main_map.player->str_out = "";
            if(main_map.player->hp == 0) {
                continue;
            }
        }
        if(changeable) {
            changeable = false;
        }
    } // if
}
