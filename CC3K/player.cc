//
// Created by JamesXu on 2020/8/4.
//

#include "map.h"
#include "player.h"
#include <cmath>
#include <iostream>

using namespace std;

//player ctor
Player::Player(string race,Posn pos,Map * map) : level{map->level} {
    if( race == "Shade" ){
        this->race = "Shade";
        this->attack_num = 25;
        this->hp = 125;
        this->defence =25;

    }else if(race == "Drow" ){
        this->race = "Drow";
        this->attack_num = 25;
        this->hp = 150;
        this->defence =15;

    }else if(race == "Vampire"){
        this->race = "Vampire";
        this->attack_num = 25;
        this->hp = 50;
        this->defence =25;

    }else if(race == "Troll"){
        this->race = "Troll";
        this->attack_num = 25;
        this->hp = 120;
        this->defence =15;

    }else if(race == "Goblin"){
        this->race = "Goblin";
        this->attack_num = 15;
        this->hp = 110;
        this->defence =20;

    }
    //need replace later!!!
    this->Gold_num = 0;

    this->pos = pos;
    base_data["attack_num"] =this->attack_num;

    base_data["defence"] =this->defence;

    buffed_data["attack_num"] =this->attack_num;

    buffed_data["defence"] =this->defence;
    this->whole_Map = map;


}

//add_gold based on gold name
void add_gold(string gold_name,Player * player){
    if(gold_name == "normal gold pile"){
        player->Gold_num += 2;
        player->str_out += "you pick normal gold +2 ";
    }else if(gold_name == "small hoard"){
        player->Gold_num += 1;
        player->str_out += "you pick small hoard gold +1 ";
    }else if(gold_name == "merchant hoard"){
        player->Gold_num += 4;
        player->str_out += "you pick merchant hoard gold +4 ";
    }else if(gold_name == "Dragon_hoard"){
        player->Gold_num += 6;
        player->str_out += "you pick dragon hoard gold +6 ";
    }else if(gold_name == "human hoard"){
        player->Gold_num += 4;
        player->str_out += "you pick human hoard gold +2*2=4 ";
    }

}

//give direction and compute the item posn
Posn give_posn(int dir,Posn player_pos){
    int player_up_down = player_pos.x;
    int player_left_right= player_pos.y;
    Posn item_pos{-1,-1};
    if(dir == 1){
        item_pos = Posn(player_up_down + 1 , player_left_right -1);
    }else if(dir == 2){
        item_pos = Posn(player_up_down + 1 , player_left_right);
    }else if(dir == 3){
        item_pos = Posn(player_up_down + 1 , player_left_right +1);
    }else if(dir == 4){
        item_pos = Posn(player_up_down  , player_left_right -1);
    }else if(dir == 5){
        item_pos = Posn(player_up_down  , player_left_right +1);
    }else if(dir == 6){
        item_pos = Posn(player_up_down - 1 , player_left_right -1);
    }else if(dir == 7){
        item_pos = Posn(player_up_down - 1 , player_left_right);
    }else if(dir == 8){
        item_pos = Posn(player_up_down - 1 , player_left_right +1);
    }
    return item_pos;
}

//helper player move
void player_move_helper2(int dir,Player * player){
    // std::cout<<"reach"<<std::endl;

    char block = player->surround[dir-1];
    //std::cout<<"block: "<<block<<std::endl;

    if(block != '.' && block != '+' && block != '#' && block !='G' && block !='\\' && block !='9'){
        // std::cout<<"reach2"<<std::endl;
        player->str_out += "there is a wall or moster you cant go there ";
        return;
    }else if(block =='G'){
        Posn gold_pos = give_posn(dir,player->pos);
        // std::cout<<"we need find pos "<<std::endl;
        //          std::cout<<"x: "<<gold_pos.x<<" y: "<<gold_pos.y<<std::endl;

        //find normal gold
        int index = 0;
        //debug print (print all the gold)
        /*
         for(auto n:player->whole_Map->golds){
         std::cout<<n->getName()<<std::endl;
          std::cout<<"x: "<<n->getPosn().x<<" y: "<<n->getPosn().y<<std::endl;
         }
         */
        for(auto n:player->whole_Map->golds){

            if(n->getPosn() == gold_pos){

                add_gold(n->getName(),player);
                // std::cout<<"inedx"<<index<<std::endl;


                player->whole_Map->golds.erase(player->whole_Map->golds.begin()+index);

                /*
                for(auto n:player->whole_Map->golds){
                   std::cout<<n->getName()<<std::endl;
                   std::cout<<"x: "<<n->getPosn().x<<" y: "<<n->getPosn().y<<std::endl;
                    }
               // std::cout<<"reach7"<<std::endl;
               */
                player->whole_Map->update();
                break;

            }else{
                index++;
            }
        }
        Posn gold_pos2 = give_posn(dir,player->pos);
        //find dragon hord part
        int index2 = 0;
        for(auto n:player->whole_Map->enemies){
            if(n->getPosn() == gold_pos2){
                //std::cout<<"reach3"<<std::endl;
                //std::cout<<"inedx2"<<index2<<std::endl;

                if(n->getRace()=="Dragon_hoard"){
                    //std::cout<<"reach4"<<std::endl;
                    if(n->attack_num == 666){
                        add_gold(n->getRace(),player);
                        player->whole_Map->enemies.erase(player->whole_Map->enemies.begin()+index2);
                        player->whole_Map->update();
                        break;
                    }else {
                        //std::cout<<"reach4"<<std::endl;
                        player->str_out += "| this is a dragon hord and dragon not die yet you cant pick up ";


                    }
                }


            }else{
                index2++;
            }

        }
    }


    else if (block == '\\') {
        player->level ++;
        char level = '0'+player->level;
        player->str_out = " you now go to level ";
        player->str_out += level;
        player->str_out += " ";
        return;
    }

    if(dir == 1){
        player->pos.x = player->pos.x + 1;
        player->pos.y = player->pos.y - 1;
        player->str_out += "you go down left " ;
    }else if(dir == 2){
        player->pos.x = player->pos.x + 1;

        player->str_out += "you go down " ;
    }else if(dir == 3){
        player->pos.x = player->pos.x + 1;
        player->pos.y = player->pos.y + 1;
        player->str_out += "you go down right " ;
    }else if(dir == 4){

        player->pos.y = player->pos.y - 1;
        player->str_out += "you go left " ;
    }else if(dir == 5){

        player->pos.y = player->pos.y + 1;
        player->str_out += "you go right " ;
    }else if(dir == 6){
        player->pos.x = player->pos.x - 1;
        player->pos.y = player->pos.y - 1;
        player->str_out += "you go up left " ;
    }else if(dir == 7){
        player->pos.x = player->pos.x - 1;

        player->str_out += "you go up " ;
    }else if(dir == 8){
        player->pos.x = player->pos.x - 1;
        player->pos.y = player->pos.y + 1;
        player->str_out += "you go up right " ;
    }

    player->whole_Map->update();
}


//the implement of player move
void Player::move(string str) {
    this->generate_surround();

    if(str == "no"){
        player_move_helper2(7,this);

    }else if(str == "so"){
        player_move_helper2(2,this);


    }else if(str == "ea"){
        player_move_helper2(5,this);


    }else if(str == "we"){
        player_move_helper2(4,this);


    }else if(str == "ne"){
        player_move_helper2(8,this);


    }else if(str == "nw"){
        player_move_helper2(6,this);



    }else if(str == "se"){
        player_move_helper2(3,this);


    }else if(str == "sw"){
        player_move_helper2(1,this);
    }

}

//add hp based on race
void add_hp(Player * player){
    string messages = " RH potion make you become FULL HP (cant over MAX HP) ";
    if(player->getRace() == "Shade"){
        if(player->hp + 10 > 125){
            player->hp = 125;
            player->str_out += messages;
        }else{
            player->hp += 10;
        }
    }else if(player->getRace() == "Drow"){
        if(player->hp + 10 > 150){
            player->hp = 150;
            player->str_out += messages;
        }else{
            player->hp += 10;
        }
    }else if(player->getRace() == "Troll"){
        if(player->hp + 10 > 120){
            player->hp = 120;
            player->str_out += messages;
        }else{
            player->hp += 10;
        }
    }else if(player->getRace() == "Goblin"){
        if(player->hp + 10 > 110){
            player->hp = 110;
            player->str_out += messages;
        }else{
            player->hp += 10;
        }
    }else if(player->getRace() == "Vampire"){
        player->hp += 10;
    }

}

//add potion buff based on potion name
void add_potion(string potion_name,Player * player){

    if(potion_name == "RH"){

        add_hp(player);
        if(player->getRace()=="Drow"){
            player->str_out +=" this is a RH potion you hp +15 ";
        }else{
            player->str_out +=" this is a RH potion you hp +10 ";
        }
    }else if(potion_name == "BA"){
        player->potions_list.emplace_back(make_shared<Potion>("BA",Posn{-1,-1}));
        player->analyse_buff();
        if(player->getRace()=="Drow"){
            player->str_out +=" this is a BA potion you attack +7 ";
        }else{
            player->str_out +=" this is a BA potion you attack +5 ";
        }
    }else if(potion_name == "BD"){
        player->potions_list.emplace_back(make_shared<Potion>("BD",Posn{-1,-1}));
        player->analyse_buff();
        if(player->getRace()=="Drow"){
            player->str_out +=" this is a BD potion you defence +7 ";
        }else{
            player->str_out +=" this is a BD potion you defence +5 ";
        }
    }else if(potion_name == "PH"){
        if(player->getRace()=="Drow"){
            player->str_out +=" this is a PH potion you hp -15 ";
            player->hp -= 15;

            if(player->hp <= 0){
                player->die();
            }
        }else{
            player->hp -= 10;
            player->str_out +=" this is a PH potion you hp -10 ";
            if(player->hp <= 0){
                player->die();
            }
        }

    }else if(potion_name == "WA"){
        player->potions_list.emplace_back(make_shared<Potion>("WA",Posn{-1,-1}));
        player->analyse_buff();
        if(player->getRace()=="Drow"){
            player->str_out +=" this is a WA potion you attack -7 ";
        }else{
            player->str_out +=" this is a WA potion you attack -5 ";
        }
    }else if(potion_name == "WD"){
        player->potions_list.emplace_back(make_shared<Potion>("WD",Posn{-1,-1}));
        player->analyse_buff();
        if(player->getRace()=="Drow"){
            player->str_out +=" this is a WD potion you defence -7 ";
        }else{
            player->str_out +=" this is a WD potion you defence -5 ";
        }
    }

}

void player_use_helper(int dir,Player * player){
    char block = player->surround[dir-1];
    //std::cout<<"block: "<<block<<std::endl;

    if(block == 'P'){
        //we get a valid potion!
        Posn potion_pos = give_posn(dir,player->pos);
        int index = 0;
        for(auto n:player->whole_Map->potions){
            if(n->getPosn() == potion_pos){
                add_potion(n->getName(),player);
                player->whole_Map->potions.erase(player->whole_Map->potions.begin()+index);
                player->whole_Map->update();
                break;


            }else{
                index++;
            }
        }



    }else{
        //if it's not a potion
        player->str_out += " it's not a potion i cant use it! ";
    }

}

//the implement of all the use
void Player::use(string str){
    this->generate_surround();

    if(str == "no"){
        player_use_helper(7,this);

    }else if(str == "so"){
        player_use_helper(2,this);


    }else if(str == "ea"){
        player_use_helper(5,this);


    }else if(str == "we"){
        player_use_helper(4,this);


    }else if(str == "ne"){
        player_use_helper(8,this);


    }else if(str == "nw"){
        player_use_helper(6,this);



    }else if(str == "se"){
        player_use_helper(3,this);


    }else if(str == "sw"){
        player_use_helper(1,this);
    }

}

//modify the hp and check if enemy die or not
void attack_die(int damage,shared_ptr<Enemy> enemy){

    if (enemy->hp - damage <= 0 ){
        if(enemy->whole_Map->player->getRace()=="Goblin"){
            enemy->whole_Map->player->Gold_num += 5;
            enemy->whole_Map->player->str_out += " Goblin will steals 5 gold for killing an enemy! ";
        }
        enemy->die();
    }else{

        enemy->hp -= damage;
        string name = enemy->getRace();
        string num = to_string(damage);
        string left_hp = to_string(enemy->hp);
        enemy->whole_Map->player->str_out += "| you attack "+ name+" his hp -"+ num +" still have hp:"+left_hp +" ";
    }
}

//generate 0 or 1
int random_num_gen2(Map * map){
    std::srand(map->seed);
    int i;
//i = rand() + 1 ;
    i = (rand()%2)  ;
    map->seed += rand();

    return i;
}

//damage_helper calculate damgae
//argument are def of defender and atk of attacker
int damage_helper2(double def,double atk){
    double first_part =  (100/(100+def));
    double second_part =  first_part*atk;
    //std::cout<<"damage cal: "<<first_part<<std::endl;
    return ceil(second_part);

}


void player_attack(Player * player, shared_ptr<Enemy> enemy){
    if(enemy->getRace() == "Merchant"){
        for(auto n :player->whole_Map->enemies){
            n->attacked = true;
        }
        player->str_out += " |now every Merchant this floor is enemy now! ";
    }

    if(player->getRace() == "Vampire"){
        int damage = damage_helper2(enemy->defence,player->buffed_data["attack_num"]);
        attack_die(damage,enemy);
        if(enemy->getRace() == "Dwarf"){
            if( player->hp -5 <0){
                player->die();
            }else{
                player->str_out += " Vampire will lose 5 hp when atk Dwarf! ";
                player->hp -= 5;
            }
        }else{
            player->str_out += " Vampire will gain 5 hp after each success atk! ";
            player->hp += 5;
        }

    }else{
        int damage = damage_helper2(enemy->defence,player->buffed_data["attack_num"]);
        attack_die(damage,enemy);
    }

}

//a helper that attack enenmy based on direction
void player_attack_helper(int dir,Player * player){
    char block = player->surround[dir-1];
    //std::cout<<"block: "<<block<<std::endl;

    if(block == 'H' || block =='W' || block =='E' || block =='O' || block =='M' || block =='L' || block =='D'){
        //we get a valid enemy!
        Posn enemy_pos = give_posn(dir,player->pos);
        int index = 0;
        for(auto n:player->whole_Map->enemies){
            if(n->getPosn() == enemy_pos){

                if(n->getRace() == "Halfling"){
                    int chance = random_num_gen2(player->whole_Map);
                    if(chance == 0){
                        player->str_out += " Halfling have 50% chance miss! you miss! ";
                    }else{
                        player->str_out += " Halfling have 50% chance miss! you not miss this time! ";
                        player_attack(player,n);
                        break;
                    }

                }else{
                    player_attack(player,n);
                    // player->whole_Map->enemies.erase(player->whole_Map->enemies.begin()+index);
                    // player->whole_Map->update();
                    break;
                }


            }else{
                index++;
            }
        }



    }else{
        //if it's not a potion
        player->str_out += " it's not a enemy i cant attack it! ";
    }


}

void Player::attack(string str) {
    this->generate_surround();
    if(str == "no"){
        player_attack_helper(7,this);

    }else if(str == "so"){
        player_attack_helper(2,this);


    }else if(str == "ea"){
        player_attack_helper(5,this);


    }else if(str == "we"){
        player_attack_helper(4,this);


    }else if(str == "ne"){
        player_attack_helper(8,this);


    }else if(str == "nw"){
        player_attack_helper(6,this);



    }else if(str == "se"){
        player_attack_helper(3,this);


    }else if(str == "sw"){
        player_attack_helper(1,this);
    }

}

//this is what happened when player die
void Player::die() {
    this->str_out = "";
    this->hp = 0;

    int score = this->Gold_num;
    if(this->getRace()== "Shade"){
        score *= 1.5;
    }
    std::string str_score = to_string(score);
    std::string message;
    message = " YOU DIE! Your Score: "  + str_score +"| You can press r to restart or q to quit";
    str_out = message;

}

void Player::analyse_buff(){
    std::string message = " Drow will have 1.5x potion effects ";
    int size = potions_list.size();
    auto n = potions_list.at(size-1);

    if(n->getName() == "BA"){
        if(this->getRace()=="Drow"){
            buffed_data["attack_num"] += 7;
            this->str_out +=  message;
        }else{
            buffed_data["attack_num"] += 5;
        }
    }else if(n->getName() == "BD"){
        if(this->getRace()=="Drow"){
            buffed_data["defence"] += 7;
            this->str_out +=  message;
        }else{
            buffed_data["defence"] += 5;
        }
    }else if(n->getName() == "WA"){
        if(this->getRace()=="Drow"){
            this->str_out +=  message;
            if( (buffed_data["attack_num"]-7) < 0){
                buffed_data["attack_num"] = 0;
            }else{

                buffed_data["attack_num"] -= 7;
            }
        }else{
            if( (buffed_data["attack_num"]-5) < 0){
                buffed_data["attack_num"] = 0;
            }else{

                buffed_data["attack_num"] -= 5;
            }
        }
    }else if(n->getName() == "WD"){
        if(this->getRace()=="Drow"){
            this->str_out +=  message;
            if( (buffed_data["defence"]-7) < 0){
                buffed_data["defence"] = 0;
            }else{
                buffed_data["defence"] -= 7;
            }
        }else{
            if( (buffed_data["defence"]-5) < 0){
                buffed_data["defence"] = 0;
            }else{
                buffed_data["defence"] -= 5;
            }
        }
    }

}


