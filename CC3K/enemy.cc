#include "enemy.h"
#include <cstdlib>
#include "map.h"
#include <ctime>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

//change pos based on command
void walk_helper(Character * c, Map * map,int dir){
    if(dir == 1){
        c->pos.x = c->pos.x + 1 ;
        c->pos.y = c->pos.y - 1 ;
    }else if(dir == 2){
        c->pos.x = c->pos.x + 1 ;

    }else if(dir == 3){
        c->pos.x = c->pos.x + 1 ;
        c->pos.y = c->pos.y + 1 ;
    }else if(dir == 4){

        c->pos.y = c->pos.y - 1 ;
    }else if(dir == 5){

        c->pos.y = c->pos.y + 1 ;
    }else if(dir == 6){
        c->pos.x = c->pos.x - 1 ;
        c->pos.y = c->pos.y - 1 ;
    }else if(dir == 7){
        c->pos.x = c->pos.x - 1 ;

    }else if(dir == 8){
        c->pos.x = c->pos.x - 1 ;
        c->pos.y = c->pos.y + 1 ;
    }
    map->update();




}

// a helper that let character move
void random_walk(Character * c,Map* map){
//std::cout<<"seed: "<<map->seed<<std::endl;
    std::srand(map->seed);
    int i;
//i = rand() + 1 ;
    i = (rand()%8) + 1 ;
    map->seed += rand();


    int next_possible_dir = -1;
    int index = 1;
    //for debug
/*
     for(auto n:c->surround){
          std::cout<<" find n: "<<n <<std::endl;
     }
*/

    for(auto n:c->surround){
        if(n == '.'){

            next_possible_dir = index;
            // std::cout<<" find next_possible_dir: "<<next_possible_dir <<std::endl;
            // std::cout<<std::endl;
            break;
        }else{
            index = index + 1;
        }
    }

    if(next_possible_dir == -1){
        //   std::cout<<"not find next_possible_dir: "<<std::endl;
        return;
    }
    if(c->surround.at(i-1) != '.'){
        walk_helper(c,map,next_possible_dir);
    }
    else{
        walk_helper(c,map,i);
    }

//std::cout<<"i = " <<i<<std::endl;


}

void Enemy::move(string) {
    this->generate_surround();
    //for merchant and dragon
    if(this->race == "Dragon"){
        return;
    }
    if(this->race == "Dragon_hoard"){
        return;
    }
    if(this->race == "Merchant"){
        if(this->attacked == false){
            return;
        }
        else if(this->attacked == true){
            random_walk(this,whole_Map);

        }
    }

    random_walk(this,whole_Map);
}

//damage_helper calculate damgae
//argument are def of defender and atk of attacker
int damage_helper(double def,double atk){
    double first_part =  (100/(100+def));
    double second_part =  first_part*atk;
    //std::cout<<"damage cal: "<<first_part<<std::endl;
    return ceil(second_part);

}

//generate 0 or 1
int random_num_gen(Map * map){
    std::srand(map->seed);
    int i;
//i = rand() + 1 ;
    i = (rand()%2)  ;
    map->seed += rand();

    return i;
}

//a helper that for normal_attack player
void normal_attack_helper(Character * c,Map * map){
    std::string name = c->getRace();
    map->player->str_out += "| "+name + " try to attack you ";
    int atk_result = random_num_gen(map);

    if(atk_result == 0){
        map->player->str_out += "but attack fail ";
    }else if(atk_result == 1 ){


        int damage = damage_helper(map->player->buffed_data["defence"],c->attack_num);

        if(c->getRace()== "Orc" && map->player->getRace()=="Goblin"){
            damage *= 1.5;
            map->player->str_out +=" Orc will casues 1.5x damage to Goblin! ";
        }

        if(map->player->hp - damage <= 0){
            map->player->die();
        }else{
            map->player->hp -= damage;
            string num = to_string(damage);
            map->player->str_out += "and you hp -" + num+" ";
        }
    }
}

void  Enemy::attack(string){
    this->generate_surround();
    bool attacked = false;
    for(auto n:this->surround){
        if(n == '@'){


            normal_attack_helper(this,this->whole_Map);
            attacked = true;
            break;
        }

    }

    if(attacked == false){
        if(this->whole_Map->freeze == false){
            this->move("");

        }

    }


}

//a helper remove enemy from map (for die)
void remove_from_enemy(Character * enemy){
    Posn enemy_pos = enemy->pos;
    int index = 0;

    for(auto n: enemy->whole_Map->enemies){

        if(n->getPosn() == enemy_pos){


            enemy->whole_Map->enemies.erase(enemy->whole_Map->enemies.begin()+index);


            enemy->whole_Map->update();

            break;

        }else{
            index++;
        }
    }


}

void Enemy::die() {

    string name = this->getRace();
    int chance = random_num_gen(this->whole_Map);
    string message;
    Map * map = this->whole_Map;
    remove_from_enemy(this);


    if(chance == 0){
        map->player->Gold_num += 1;
        message = " it drop a small hoard gold +1 ";
    }else{
        map->player->Gold_num += 2;
        message = " it drop a noraml hoard gold +2 ";
    }
    map->player->str_out +="| You killed a "+ name + message;




}


Human::Human(Posn pos2,Map* map){
    this->race = "Human";
    this->hp = 140;
    this->attack_num = 20;
    this->defence =20;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

void Human::die(){
    Map * map =this->whole_Map;
    auto  player =this->whole_Map->player;
    Posn enemy_pos = this->pos;
    this->generate_surround();
    std::vector<char> surround =this->surround;
    remove_from_enemy(this);
    int available_pos=-1;
    int index = 1;

    for(auto n:surround){
        if(n == '.'){
            available_pos = index;
            break;
        }else{
            index ++;
        }

    }
    //std::cout<<"available_pos: "<<available_pos<<std::endl;
    if(available_pos == -1){
        player->str_out +=" no available place to generate 2 pile of normal gold so generate 2*2 at body place ";
        map->golds.emplace_back(make_shared<Gold>("human hoard",enemy_pos));

    }else{
        int up_down = enemy_pos.x;
        int left_right = enemy_pos.y;
        if(available_pos == 1){
            up_down ++;
            left_right --;
        }else if(available_pos == 2){
            up_down ++;

        }else if(available_pos == 3){
            up_down ++;
            left_right ++;
        }else if(available_pos == 4){

            left_right --;
        }else if(available_pos == 5){

            left_right ++;
        }else if(available_pos == 6){
            up_down --;
            left_right --;
        }else if(available_pos == 7){
            up_down --;

        }else if(available_pos == 8){
            up_down --;
            left_right ++;
        }
        map->golds.emplace_back(make_shared<Gold>("normal gold pile",enemy_pos));
        map->golds.emplace_back(make_shared<Gold>("normal gold pile",Posn{up_down,left_right}));
        player->str_out +="|human be killed drop 2 normal hoard ";
    }



}

Dwarf::Dwarf(Posn pos2,Map* map){
    this->race = "Dwarf";
    this->hp = 100;
    this->attack_num = 20;
    this->defence =30;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

void Dwarf::attack(string){
    this->generate_surround();
    bool attacked = false;
    for(auto n:this->surround){
        if(n == '@'){
            normal_attack_helper(this,this->whole_Map);
            attacked = true;
            break;
        }

    }

    if(attacked == false){
        if(this->whole_Map->freeze == false){
            this->move("");

        }

    }


}

Elf::Elf(Posn pos2,Map* map){
    this->race = "Elf";
    this->hp = 140;
    this->attack_num = 30;
    this->defence =10;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

void Elf::attack(string){
    this->generate_surround();
    bool attacked = false;
    for(auto n:this->surround){
        if(n == '@'){
            if(this->getRace() == "Elf" ){
                //std::cout<<"reach4"<<std::endl;
                if(this->whole_Map->player->getRace()=="Drow"){

                    this->whole_Map->player->str_out += " Elf attack twice each round except Drow ";
                }else{
                    normal_attack_helper(this,this->whole_Map);
                    this->whole_Map->player->str_out += " Elf attack twice each round ";
                }
            }
            normal_attack_helper(this,this->whole_Map);
            attacked = true;
            break;
        }

    }

    if(attacked == false){
        if(this->whole_Map->freeze == false){
            this->move("");

        }

    }

}

Orc::Orc(Posn pos2,Map* map){
    this->race = "Orc";
    this->hp = 180;
    this->attack_num = 30;
    this->defence =25;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

void Orc::attack(string){
    this->generate_surround();
    bool attacked = false;
    for(auto n:this->surround){
        if(n == '@'){
            normal_attack_helper(this,this->whole_Map);
            attacked = true;
            break;
        }

    }

    if(attacked == false){
        if(this->whole_Map->freeze == false){
            this->move("");

        }

    }
}

Merchant::Merchant(Posn pos2,Map* map){
    this->race = "Merchant";
    this->hp = 30;
    this->attack_num = 70;
    this->defence =5;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

void Merchant::attack(string){
    if(this->attacked == false){
        return;
    }else{
        this->generate_surround();
        bool attacked = false;
        for(auto n:this->surround){
            if(n == '@'){
                normal_attack_helper(this,this->whole_Map);
                attacked = true;
                break;
            }

        }

        if(attacked == false){
            if(this->whole_Map->freeze == false){
                this->move("");

            }

        }
    }
}



void Merchant::die() {
    Posn enemy_pos = this->pos;
    int index = 0;
    for(auto n: this->whole_Map->enemies){

        if(n->getPosn() == enemy_pos){


            this->whole_Map->enemies.erase(this->whole_Map->enemies.begin()+index);


            this->whole_Map->update();
            break;

        }else{
            index++;
        }
    }
    this->whole_Map->player->str_out +="| you kill a Merchant! he drop a merchant hoard ";
    this->whole_Map->golds.emplace_back(make_shared<Gold>("merchant hoard",enemy_pos));



}

Halfling::Halfling(Posn pos2,Map* map){
    this->race = "Halfling";
    this->hp = 100;
    this->attack_num = 15;
    this->defence =20;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

Dragon::Dragon(Posn pos2,Map* map){
    this->race = "Dragon";
    this->hp = 150;
    this->attack_num = 20;
    this->defence =20;
    attacked = false;
    this->pos = pos2;
    this->whole_Map = map;
}

void Dragon::attack(string str){

    this->generate_surround();
    this->ptr_to_Dargon_hoard->generate_surround();
    for(auto n :this->ptr_to_Dargon_hoard->surround){
        this->surround.emplace_back(n);
    }

    for(auto n:this->surround){
        if(n == '@'){
            this->whole_Map->player->str_out += "|warning! dragon noticed you! and he will attack you!";
            normal_attack_helper(this,this->whole_Map);

            break;
        }

    }

}

void Dragon::die(){
    this->ptr_to_Dargon_hoard->attack_num = 666;
    int index = 0;
    Posn enemy_pos = this->pos;
    for(auto n: this->whole_Map->enemies){

        if(n->getPosn() == enemy_pos){


            this->whole_Map->enemies.erase(this->whole_Map->enemies.begin()+index);

            this->whole_Map->player->str_out += " |you kill the dragon now you can pickup dragon hoard! ";
            this->whole_Map->update();
            break;

        }else{
            index++;
        }
    }
}


Dragon_hoard::Dragon_hoard(Posn pos2,Map* map){
    this->race = "Dragon_hoard";
    this->hp = 0;
    this->attack_num = 0;
    this->defence =0;

    this->pos = pos2;
    this->whole_Map = map;

}


void Dragon_hoard::attack(string){
    for(auto n:this->surround){
        if(n == '@'){
            if(attack_num == 666){
                whole_Map->player->str_out += " this is a Dragon Hoard area and you kill the dragon go pick your Gold! ";
            }else{
                whole_Map->player->str_out += " this is a Dragon Hoard area dragon will attack you! ";
            }
            break;
        }

    }

}
