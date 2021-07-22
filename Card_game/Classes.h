#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <map>
#include <math.h>
#include <thread>
#include <conio.h>
#include <windows.h>

using namespace std;
class menu;
class player;
class Card {
public:
	string state;
	string type;
	int anykey;
	int card_id;
	int move_cost;
	int special_move_cost;
	int owner;
	int health;
	int full_hp;
	int dmg;
	int def;
	int attack;
	virtual void attack_enemy(Card* &obj1, player &obj2, player &obj3) = 0;
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3) = 0;
	bool is_dead() {
		if (this->health < 1) return true;
		else return false;
	}
};
class Warrior : public Card {
public:
	static const int up_mana_cost = 20;
	static const int mana_cost = 15;
	static const int upgrade_move_cost = 1;
	Warrior(int own, int number) {
		this->anykey = 1;
		this->special_move_cost = 0;
		this->state = "normal";
		this->full_hp = 80;
		this->move_cost = 2;
		this->type = "Warrior";
		this->health = 80;
		this->dmg = 12;
		this->attack = 15;
		this->def = 30;
		if (own > 0 && own < 3) this->owner = own;
		this->card_id = number;
	}
	virtual void attack_enemy(Card* &obj1, player &obj2, player &obj3);
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3);
};
class Archer : public Card {
public:
	static const int mana_cost = 20;
	Archer(int own, int number) {
		this->anykey = 2;
		this->special_move_cost = 0;
		this->state = "normal";
		this->full_hp = 50;
		this->move_cost = 1;
		this->type = "Archer";
		this->health = 50;
		this->dmg = 20;
		this->attack = 10;
		this->def = 10;
		if (own > 0 && own < 3) this->owner = own;
		this->card_id = number;
	}
	virtual void attack_enemy(Card* &obj1, player &obj2, player &obj3);
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3);
};
class Mage : public Card {
public:
	static const int mana_cost = 25;
	Mage(int own, int number) {
		this->anykey = 4;
		this->special_move_cost = 2;
		this->state = "normal";
		this->full_hp = 40;
		this->move_cost = 2;
		this->type = "Mage";
		this->health = 40;
		this->dmg = 10;
		this->attack = 10;
		this->def = 6;
		if (own > 0 && own < 3) this->owner = own;
		this->card_id = number;
	}
	virtual void attack_enemy(Card* &obj1, player &obj2, player &obj3);
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3);
	void fear(Card* obj1) {
		obj1->state = "scared";
	}
	void iron_maiden(Card* obj1) {
		obj1->state = "cursed";
	}
};
class Assassin : public Card {
public:
	static const int mana_cost = 25;
	Assassin(int own, int number) {
		this->anykey = 0;
		this->special_move_cost = 2;
		this->state = "normal";
		this->full_hp = 35;
		this->move_cost = 2;
		this->type = "Assassin";
		this->health = 35;
		this->dmg = 28;
		this->attack = 20;
		this->def = 15;
		if (own > 0 && own < 3) this->owner = own;
		this->card_id = number;
	}
	void attack_enemy(Card* &obj1, player &obj2, player &obj3);
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3);
	void fade() {
		this->state = "untargetable";
	}
	void assassination(player &obj2);
};
class Priest : public Card {
public:
	static const int mana_cost = 20;
	Priest(int own, int number) {
		this->anykey = 2;
		this->special_move_cost = 1;
		this->state = "normal";
		this->full_hp = 40;
		this->move_cost = 2;
		this->type = "Priest";
		this->health = 40;
		this->dmg = 8;
		this->attack = 8;
		this->def = 4;
		if (own > 0 && own < 3) this->owner = own;
		this->card_id = number;
	}
	virtual void attack_enemy(Card* &obj1, player &obj2, player &obj3);
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3);
	void purify(Card* obj1) {
		if (obj1->state != "untargetable") obj1->state = "normal";
		else cout << "cannot purify an untargetable unit, it can only be cursed" << endl;
	}
	void heal(Card* obj1) {
		obj1->health = obj1->full_hp;
	}
};
class Bard : public Card {
public:
	static const int mana_cost = 20;
	Bard(int own, int number) {
		this->anykey = 2;
		this->special_move_cost = 2;
		this->state = "normal";
		this->full_hp = 40;
		this->move_cost = 2;
		this->type = "Bard";
		this->health = 40;
		this->dmg = 10;
		this->attack = 10;
		this->def = 8;
		if (own > 0 && own < 3) this->owner = own;
		this->card_id = number;
	}
	virtual void attack_enemy(Card* &obj1, player &obj2, player &obj3);
	virtual void special_abilities(Card* &obj1, player &obj2, player &obj3);
	void convert(player &obj1, player &obj2);
};
class turn {
public:
	bool who = true;
	void switch_turn() {
		if (this->who) this->who = false;
		else this->who = true;
		this->read_turn();
	}
	void read_turn() {
		if (who) cout << "it's player 1's turn" << endl;
		else cout << "it'splayer 2's turn" << endl;
	}
};
class player {
public:
	int id;
	int mana;
	int hp;
	int deck_size;
	int move_points;
	bool board_flag = false;
	bool cls_flag = false;
	bool drawer_finish_flag = false;
	thread drawer;
	map <int, Card*> Deck;
	string number;
	string summoned_card_name;
	player(int iden) {
		this->move_points = 5;
		this->id = iden;
		this->mana = 250;
		this->hp = 500;
		this->deck_size = 0;
	}
	void sig() {
		cls_flag = true;
		Sleep(100);
		cls_flag = false;
	}
	void display_deck(player &obj2) {
		system("cls");
		int iterator = 1;
		int it = -5;
		int size = 4;
		int max_hp = 0;
		bool flag = false;
		cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "YOUR DECK                                                                                                    ENEMY DECK" << endl;
		cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
		while (this->Deck[iterator] != NULL || obj2.Deck[iterator] != NULL) {
			if (iterator > 9) flag = true;
			if (this->Deck[iterator] != NULL) {
				it = -5;
				if (this->Deck[iterator]->anykey < size) size = this->Deck[iterator]->anykey;
				if (this->Deck[iterator]->health > max_hp) max_hp = this->Deck[iterator]->health;
				if (this->Deck[iterator]->type != "Elite Warrior") cout << "    " << iterator << ". " << this->Deck[iterator]->type << ", " << this->Deck[iterator]->state << " hp: " << this->Deck[iterator]->health << "/" << this->Deck[iterator]->full_hp << "                                                   ";
				else cout << "    " << iterator << ". " << this->Deck[iterator]->type << ", " << this->Deck[iterator]->state << " hp: " << this->Deck[iterator]->health << "/" << this->Deck[iterator]->full_hp << "                                                 ";
				while (it < this->Deck[iterator]->anykey) {
					cout << " ";
					it++;
				}it = -5;
				if (this->Deck[iterator]->health < 100 && this->Deck[iterator]->health > 9) cout << " ";
				else if (this->Deck[iterator]->health < 10) cout << "  ";
			}
			else {
				it = 0;
				if (flag == true) size--;
				while (it < size) {
					cout << " ";
					it++;
				}it = 0;
				if (flag == true) size++;
				cout << "                                                                                      ";
				if (max_hp > 9 && max_hp < 100) cout << "  ";
				else if (max_hp < 10) cout << " ";
			}
			if (obj2.Deck[iterator] != NULL) {
				cout << iterator << ". " << obj2.Deck[iterator]->type << ", " << obj2.Deck[iterator]->state << " hp: " << obj2.Deck[iterator]->health << "/" << obj2.Deck[iterator]->full_hp;
			}
			iterator++;
			cout << endl << endl;
		}
		cout << endl << "-----------------------------------------------------------------------------------------------------------------------" << endl << endl << endl;
		iterator = 0;
	}
	bool isAlive() {
		if (this->hp > 0) return true;
		else return false;
	}
	void show_stats() {
		cout << "move points availabe: " << this->move_points << endl;
		cout << "mana points available: " << this->mana << endl;
		cout << "health points remaining: " << this->hp << endl;
	}
	void move_points_restore() {
		this->move_points += 2;
		if (this->move_points > 5) this->move_points = 5;
	}
	void choose_action(turn &obj1, player &obj2, player &obj3);
	void upgrade_warrior(turn &obj1, player &obj2, player &obj3) {
		int select;
		string anykey;
		cout << "choose a warrior card you would like to upgrade: ";
		cin >> select;
		if (this->Deck[select] == NULL) {
			system("cls");
			cout << "there is no such card, try again, press anykey to continue" << endl;
			cin >> anykey;
			this->upgrade_warrior(obj1, obj2, obj3);
		}
		else if (this->Deck[select] != NULL && this->Deck[select]->type != "Warrior") {
			system("cls");
			cout << "this card is not a warrior card, try again, press anykey to continue" << endl;
			cin >> anykey;
			this->upgrade_warrior(obj1, obj2, obj3);
		}
		else {
			if (this->move_points > 0) {
				if (this->mana > 19) {
					this->move_points -= Warrior::upgrade_move_cost;
					this->mana -= Warrior::up_mana_cost;
					this->Deck[select]->type = "Elite Warrior";
					this->Deck[select]->health += 35;
					this->Deck[select]->full_hp = 115;
					this->Deck[select]->dmg = 18;
					this->Deck[select]->attack = 28;
					this->Deck[select]->def = 50;
					this->Deck[select]->anykey = -5;
				}
				else {
					system("cls");
					cout << "not enogh mana to perform this action, press anykey to continue" << endl;
					cin >> anykey;
					this->choose_action(obj1, obj2, obj3);
				}
			}
			else {
				system("cls");
				cout << "not move points to perform this action, press anykey to continue" << endl;
				cin >> anykey;
				this->choose_action(obj1, obj2, obj3);
			}
		}
	}
	void attack_enemy(turn &obj1, player &obj2, player &obj3) {
		int iterator = 1, select, temp;
		string anykey, choice;
		bool condition = true;
		while (iterator < 20) {
			if (obj2.Deck[iterator] != NULL && obj2.Deck[iterator]->is_dead() == false) {
				condition = false;
				break;
			}
			iterator++;
		}
		if (condition == false) {
			system("cls");
			cout << "you cannot perform this action, enemy still has cards left, press any key to continue: " << endl;
			cin >> anykey;
			this->choose_action(obj1, obj2, obj3);
		}
		else {
			cout << "chose a card you wish to attack with: " << endl;
			cin >> select;
			if (this->Deck[select] != NULL) {
				if (this->Deck[select]->is_dead() == false) {
					temp = obj2.hp;
					obj2.hp -= (this->Deck[select]->dmg) * 3;
					if (obj2.hp < 0) obj2.hp = 0;
					cout << "enemy's hp has been reduced from " << temp << " to " << obj2.hp << endl;
					this->move_points -= this->Deck[select]->move_cost;
					cout << "press any key to continue: " << endl;
					cin >> anykey;
				}
				else {
					cout << "this card is dead, please choose another (1) or go back to the main menu (2)" << endl;
					cin >> choice;
					if (choice == "1") this->attack_enemy(obj1, obj2, obj3);
					else if (choice == "2") this->choose_action(obj1, obj2, obj3);
					else {
						cout << "data incorrect, try again" << endl;
						this->attack_enemy(obj1, obj2, obj3);
					}
				}
			}
			else {
				system("cls");
				cout << "you do not have such a card, choose another" << endl;
				this->attack_enemy(obj1, obj2, obj3);
			}
		}
	}
	void show_card_detail(turn &obj1, player &obj2, player &obj3) {
		int select;
		string action;
		cout << "select card" << endl;
		cin >> select;
		if (this->Deck[select] != NULL) {
			cout << "type: " << this->Deck[select]->type << endl;
			cout << "current status: " << this->Deck[select]->state << endl;
			cout << "id: " << this->Deck[select]->card_id << endl;
			cout << "owner: " << this->Deck[select]->owner << endl;
			cout << "current hp: " << this->Deck[select]->health << "/" << this->Deck[select]->full_hp << endl;
			cout << "damage: " << this->Deck[select]->dmg << endl;
			cout << "defense rate: " << this->Deck[select]->def << endl;
			cout << "attack rate: " << this->Deck[select]->attack << endl;
			cout << endl << "do you want to see another card's details? (y = yes, n = main menu)" << endl;
			cin >> action;
			if (action == "y") {
				system("cls");
				this->show_card_detail(obj1, obj2, obj3);
			}
			else if (action == "n") {
				this->show_stats();
				this->choose_action(obj1, obj2, obj3);
			}
			else {
				cout << "data incorrect, try again" << endl;
				this->show_card_detail(obj1, obj2, obj3);
			}
		}
		else {
			cout << "data incorect, try again" << endl;
			this->show_card_detail(obj1, obj2, obj3);
		}
	}
	void show_deck(turn &obj1, player &obj2, player &obj3) {
		int iterator = 1;
		string anykey;
		system("cls");
		while (this->Deck[iterator] != NULL) {
			if (this->Deck[iterator]->health > 0) {
				cout << iterator << ". " << this->Deck[iterator]->type << ", current hp: " << this->Deck[iterator]->health << "/" << this->Deck[iterator]->full_hp << ", status: " << this->Deck[iterator]->state << endl;
			}iterator++;
		}
			cout << "press any key to continue" << endl;
			cin >> anykey;
			this->choose_action(obj1, obj2, obj3);
	}
	void end_turn(turn &obj1, player &obj2, player &obj3) {
		char action;
		cout << "do your want to end the turn?" << endl;
		cin >> action;
		if (action == 'y') {
			this->move_points_restore();
			this->drawer_finish_flag = true;
			if (this->board_flag) this->board_flag = false;
			this->drawer.join();
			obj1.switch_turn();
		}
		else if (action == 'n') {
			this->show_stats();
			this->choose_action(obj1, obj2, obj3);
		}
		else {
			cout << "data incorrect, try again" << endl;
			this->end_turn(obj1, obj2, obj3);
		}
	}
	void callback_function(player &obj2) {
		while (1) {
			if (this->cls_flag) {
				system("cls");
				Sleep(200);
				this->display_deck(obj2);
				Sleep(400);
			}
			if (this->drawer_finish_flag) {
				this->drawer_finish_flag = false;
				break;
			}
		}
	}
	void summon_card(turn &obj1, player &obj2, player &obj3) {
		string anykey;
		system("cls");

		if (this->move_points > 0) {
			string kind;
			cout << "what kind of card do you want to summon?" << endl;
			cout << "1. warrior - brute strength based card, no special abilities, can be upgraded to elite warrior - strongest card of all" << endl;
			cout << "2. archer - soft, high damage card, no special abilities, uses less move points to attack han others" << endl;
			cout << "3. mage - can't fight very well but cassts powerful curses on enemy cards" << endl;
			cout << "4. priest - previous' opposite, can heal allies and remove curses" << endl;
			cout << "5. assassin - very delicate but deadly unit that can heavily damage it's opponent even if his card blocks it's attack, can fade and become untargetable" << endl;
			cout << "6. bard - a tricky fihgter that can make your enemies fight for you!" << endl;
			cin >> kind;
			if (kind == "archer" && Archer::mana_cost <= this->mana || kind == "2" && Archer::mana_cost <= this->mana) {
				system("cls");
				this->deck_size++;
				this->mana = this->mana - Archer::mana_cost;
				cout << "you have summoned an archer to fight for you!" << endl;
				this->number = to_string(this->deck_size);
				Deck[deck_size] = new Archer(this->id, this->deck_size);
				this->move_points -= 1;
				//this->display_deck(obj2);
				this->sig();
				Sleep(500);
				this->choose_action(obj1, obj2, obj3);
			}
			else if (kind == "warrior" && Warrior::mana_cost <= this->mana || kind == "1" && Warrior::mana_cost <= this->mana) {
				system("cls");
				this->deck_size++;
				this->mana = this->mana - Warrior::mana_cost;
				cout << "you have summoned a warrior to fight for you!" << endl;
				this->number = to_string(this->deck_size);
				Deck[deck_size] = new Warrior(this->id, this->deck_size);
				this->move_points -= 1;
				//this->display_deck(obj2);
				this->sig();
				Sleep(500);
				this->choose_action(obj1, obj2, obj3);
			}
			else if (kind == "mage" && Mage::mana_cost <= this->mana || kind == "3" && Mage::mana_cost <= this->mana) {
				if (this->move_points > 1) {
					system("cls");
					this->deck_size++;
					this->mana = this->mana - Mage::mana_cost;
					cout << "you have summoned a mage to support you!" << endl;
					this->number = to_string(this->deck_size);
					Deck[deck_size] = new Mage(this->id, this->deck_size);
					this->move_points -= 2;
					//this->display_deck(obj2);
					this->sig();
					Sleep(500);
					this->choose_action(obj1, obj2, obj3);
				}
				else {
					cout << "you don't have enough move points left to perform this action, press anykey to continue: " << endl;
					cin >> anykey;
					system("cls");
					this->show_stats();
					this->choose_action(obj1, obj2, obj3);
				}
			}
			else if (kind == "assassin" && Assassin::mana_cost <= this->mana || kind == "5" && Assassin::mana_cost <= this->mana) {
				if (this->move_points > 1) {
					system("cls");
					this->deck_size++;
					this->mana = this->mana - Assassin::mana_cost;
					cout << "you have summoned an assassin to kill for you!" << endl;
					this->number = to_string(this->deck_size);
					Deck[deck_size] = new Assassin(this->id, this->deck_size);
					this->move_points -= 2;
					//this->display_deck(obj2);
					this->sig();
					Sleep(500);
					this->choose_action(obj1, obj2, obj3);
				}
				else {
					cout << "you don't have enough move points left to perform this action, press anykey to continue: " << endl;
					cin >> anykey;
					system("cls");
					this->show_stats();
					this->choose_action(obj1, obj2, obj3);
				}
			}
			else if (kind == "priest" && Assassin::mana_cost <= this->mana || kind == "4" && Assassin::mana_cost <= this->mana) {
				system("cls");
				this->deck_size++;
				this->mana = this->mana - Priest::mana_cost;
				cout << "you have summoned a priest to support you!" << endl;
				this->number = to_string(this->deck_size);
				Deck[deck_size] = new Priest(this->id, this->deck_size);
				this->move_points -= 1;
				//this->display_deck(obj2);
				this->sig();
				Sleep(500);
				this->choose_action(obj1, obj2, obj3);
			}
			else if(kind == "bard" && Bard::mana_cost <= this->mana || kind == "6" && Bard::mana_cost <= this->mana) {
				this->deck_size++;
				this->mana = this->mana - Bard::mana_cost;
				cout << "you have summoned a bard to fight for you!" << endl;
				this->number = to_string(this->deck_size);
				Deck[deck_size] = new Bard(this->id, this->deck_size);
				this->move_points -= 1;
				//this->display_deck(obj2);
				this->sig();
				Sleep(500);
				this->choose_action(obj1, obj2, obj3);
			}
			else {
				system("cls");
				cout << "data incorrect, try again! press any key to continue: " << endl;
				cin >> anykey;
				this->summon_card(obj1, obj2, obj3);
			}
		}
		else {
			cout << "you don't have enough move points left to perform this action, press anykey to continue: " << endl;
			cin >> anykey;
			system("cls");
			this->show_stats();
			this->choose_action(obj1, obj2, obj3);
		}
	}
	
	void attack_enemy_card(turn &obj1, player &obj2, player &obj3) {
		int select, select_enemy;
		string anykey;
		if (this->move_points > 0) {
			cout << "which card would you like to attack with?" << endl;
			cin >> select;
			if (this->Deck[select] != NULL) {
				cout << "please select enemy card you would like to attack" << endl;
				cin >> select_enemy;
				if (obj2.Deck[select_enemy] != NULL) {
					this->move_points -= this->Deck[select]->move_cost;
					this->Deck[select]->attack_enemy(obj2.Deck[select_enemy], obj2, obj3);
					cout << "you have hit enemy's card for " << this->Deck[select]->dmg << " and it's hp has been reduced to " << obj2.Deck[select_enemy]->health << " press any key to continue: " << endl;
					cin >> anykey;
					this->show_stats();
					this->choose_action(obj1, obj2, obj3);
				}
				else {
					system("cls");
					cout << "there is no such card in enemy deck, try again" << endl;
					this->attack_enemy_card(obj1, obj2, obj3);
				}
			}
			else {
				system("cls");
				cout << "you don't have such card, select again" << endl;
				this->attack_enemy_card(obj1, obj2, obj3);
			}
		}
		else {
			cout << "you don't have enough move points to perform this action, press any key to continue: " << endl;
			cin >> anykey;
			this->show_stats();
			this->choose_action(obj1, obj2, obj3);
		}
	}
	void special_ability(turn &obj1, player &obj2, player &obj3) {
		int select, select_target;
		string choice;
		string anykey;
		cout << "which card's abillity would you like to perform?" << endl;
		cin >> select;
		if (this->Deck[select] != NULL && this->Deck[select]->health > 0) {
			this->move_points -= this->Deck[select]->special_move_cost;
			cout << "choose card you want to perform an action on: (a - ally, e - enemy)" << endl;
			cin >> choice;
			if (choice == "a") {
				cout << "choose card: " << endl;
				cin >> select_target;
				if (this->Deck[select_target] != NULL && this->Deck[select_target]->health > 0) {
					this->Deck[select]->special_abilities(this->Deck[select_target], obj2, obj3);
					this->choose_action(obj1, obj2, obj3);
				}
				else if (this->Deck[select_target] != NULL && this->Deck[select_target]->health < 1) {
					system("cls");
					cout << "this card is dead and cannot have desired action performed on, choose again" << endl;
					this->special_ability(obj1, obj2, obj3);
				}
				else {
					system("cls");
					cout << "there is no such card, choose again" << endl;
					this->special_ability(obj1, obj2, obj3);
				}
			}
			
			else if (choice == "e") {
				cout << "choose card: " << endl;
				cin >> select_target;
				if (obj2.Deck[select_target] != NULL && obj2.Deck[select_target]->health > 0) {
					this->Deck[select]->special_abilities(obj2.Deck[select_target], obj2, obj3);
					this->choose_action(obj1, obj2, obj3);
				}
				else if (obj2.Deck[select_target] != NULL && obj2.Deck[select_target]->health < 1) {
					system("cls");
					cout << "this card is dead and cannot have desired action performed on, choose again" << endl;
					this->special_ability(obj1, obj2, obj3);
				}
				else {
					system("cls");
					cout << "there is no such card, choose again" << endl;
					this->special_ability(obj1, obj2, obj3);
				}
			}
			else {
				system("cls");
				cout << "data incorrect, try again" << endl;
				this->special_ability(obj1, obj2, obj3);
			}
		}
		else if (this->Deck[select] != NULL && this->Deck[select]->health < 1) {
			system("cls");
			cout << "card is dead and cannot perform desired action, choose another, press any key to continue: " << endl;
			cin >> anykey;
			this->special_ability(obj1, obj2, obj3);
		}
		else {
			cout << "no such card, choose another, press any key to continue: " << endl;
			cin >> anykey;
			this->special_ability(obj1, obj2, obj3);
		}
	}
};
class menu {
public:
	menu() {
		this->show();
	}
	void show() {
		int choice;
		cout << "1.----------PLAY-------------" << endl;
		cout << "2.----------EXIT-------------" << endl;
		cin >> choice;
		system("cls");
		this->action(choice);
	}
	void action(int act) {
		if (act == 1) {
			turn* t = new turn;
			player * First = new player(1);
			player * Second = new player(2);
			while (1) {
				if (t->who) {
					cout << "Player 1!" << endl;
					First->show_stats();
					First->choose_action(*t, *Second, *First);
				}
				else {
					cout << "Player 2!" << endl;
					Second->show_stats();
					Second->choose_action(*t, *First, *Second);
				}
			}
		}
		else if (act == 2){
				cout << "good bye!" << endl;
				exit(0);
		}
		if (act != 1 || act != 2) {
			system("cls");
			cout << "data incorrect, try again" << endl;
			this->show();
		}
	}
};
void Archer::attack_enemy(Card* &obj1, player &obj2, player &obj3) {
	if (this->is_dead()) {
		cout << "this card is dead and cannot perform desired action!" << endl;
	}
	else {
		if (obj1->state != "untargetable") {
			float temp = 0;
			int store = this->attack;
			if (this->state == "scared") store -= 16;
			if (store < 1) store = 1;
			int hit;
			float chance = (((float)store / (float)obj1->def) * 50);
			if (chance > 50)
			{
				temp = chance - 50;
				temp = sqrt(temp);
				chance = 50 + temp * 3;
			}
			else if (chance < 50)
			{
				temp = 50 - chance;
				temp = sqrt(temp);
				chance = 50 - temp * 3;
			}
			if (chance > 95) chance = 95;
			else if (chance < 5) chance = 5;
			srand(time(0));
			hit = (rand() % 100);
			if (hit <= chance) {
				int tmp = obj1->health;
				obj1->health = obj1->health - this->dmg;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
				if (obj1->health < 0) obj1->health = 0;
				if (this->state == "cursed") {
					obj3.hp -= this->dmg;
					if (obj3.hp < 0) obj3.hp = 0;
					cout << "this card is cursed and hurt you! your hp has dropped to: " << obj3.hp << endl;
				}
				cout << "(Archer attack) enemy has been hit and his hp therefore reduced from " << tmp << " to " << obj1->health << endl;
			}
			else cout << "enemy unit's armour has sucessfully blocked your card's attack and it's health remains untouched at " << obj1->health << " points" << endl;
		}
		else cout << "this unit is untargetable and didn't take any damage" << endl;
	}
}
void Warrior::attack_enemy(Card* &obj1, player &obj2, player &obj3) {
	if (this->is_dead()) {
		cout << "this card is dead and cannot perform desired action!" << endl;
	}
	else {
		if (obj1->state != "untargetable") {
			float temp = 0;
			int store = this->attack;
			if (this->state == "scared") store -= 16;
			if (store < 1) store = 1;
			int hit;
			float chance = (((float)store / (float)obj1->def) * 50);
			if (chance > 50)
			{
				temp = chance - 50;
				temp = sqrt(temp);
				chance = 50 + temp * 3;
			}
			else if (chance < 50)
			{
				temp = 50 - chance;
				temp = sqrt(temp);
				chance = 50 - temp * 3;
			}
			if (chance > 95) chance = 95;
			else if (chance < 5) chance = 5;
			srand(time(0));
			hit = (rand() % 100);
			if (hit <= chance) {
				int tmp = obj1->health;
				obj1->health = obj1->health - this->dmg;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
				if (obj1->health < 0) obj1->health = 0;
				if (this->state == "cursed") {
					obj3.hp -= this->dmg;
					if (obj3.hp < 0) obj3.hp = 0;
					cout << "this card is cursed and hurt you! your hp has dropped to: " << obj3.hp << endl;
				}
				cout << "(Warrior attack) enemy has been hit and his hp therefore reduced from " << tmp << " to " << obj1->health << endl;
			}
			else cout << "enemy unit's armour has sucessfully blocked your card's attack and it's health remains untouched at " << obj1->health << " points" << endl;
			}
		else cout << "this unit is untargetable and didn't take any damage" << endl;
	}
}
void Mage::attack_enemy(Card* &obj1, player &obj2, player &obj3) {
	if (this->is_dead()) {
		cout << "this card is dead and cannot perform desired action!" << endl;
	}
	else {
		if (obj1->state != "untargetable") {
			float temp = 0;
			int store = this->attack;
			if (this->state == "scared") store -= 16;
			if (store < 1) store = 1;
			int hit;
			float chance = (((float)store / (float)obj1->def) * 50);
			if (chance > 50)
			{
				temp = chance - 50;
				temp = sqrt(temp);
				chance = 50 + temp * 3;
			}
			else if (chance < 50)
			{
				temp = 50 - chance;
				temp = sqrt(temp);
				chance = 50 - temp * 3;
			}
			if (chance > 95) chance = 95;
			else if (chance < 5) chance = 5;
			srand(time(0));
			hit = (rand() % 100);
			if (hit <= chance) {
				int tmp = obj1->health;
				obj1->health = obj1->health - this->dmg;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
				if (obj1->health < 0) obj1->health = 0;
				if (this->state == "cursed") {
					obj3.hp -= this->dmg;
					if (obj3.hp < 0) obj3.hp = 0;
					cout << "this card is cursed and hurt you! your hp has dropped to: " << obj3.hp << endl;
				}
				cout << "(Mage attack) enemy has been hit and his hp therefore reduced from " << tmp << " to " << obj1->health << endl;
			}
			else cout << "enemy unit's armour has sucessfully blocked your card's attack and it's health remains untouched at " << obj1->health << " points" << endl;
			}
		else cout << "this unit is untargetable and didn't take any damage" << endl;
	}
}
void Priest::attack_enemy(Card* &obj1, player &obj2, player &obj3) {
	if (this->is_dead()) {
		cout << "this card is dead and cannot perform desired action!" << endl;
	}
	else {
		if (obj1->state != "untargetable") {
			float temp = 0;
			int store = this->attack;
			if (this->state == "scared") store -= 16;
			if (store < 1) store = 1;
			int hit;
			float chance = (((float)store / (float)obj1->def) * 50);
			if (chance > 50)
			{
				temp = chance - 50;
				temp = sqrt(temp);
				chance = 50 + temp * 3;
			}
			else if (chance < 50)
			{
				temp = 50 - chance;
				temp = sqrt(temp);
				chance = 50 - temp * 3;
			}
			if (chance > 95) chance = 95;
			else if (chance < 5) chance = 5;
			srand(time(0));
			hit = (rand() % 100);
			if (hit <= chance) {
				int tmp = obj1->health;
				obj1->health = obj1->health - this->dmg;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
				if (obj1->health < 0) obj1->health = 0;
				if (this->state == "cursed") {
					obj3.hp -= this->dmg;
					if (obj3.hp < 0) obj3.hp = 0;
					cout << "this card is cursed and hurt you! your hp has dropped to: " << obj3.hp << endl;
				}
				cout << "(Mage attack) enemy has been hit and his hp therefore reduced from " << tmp << " to " << obj1->health << endl;
			}
			else cout << "enemy unit's armour has sucessfully blocked your card's attack and it's health remains untouched at " << obj1->health << " points" << endl;
			}
		else cout << "this unit is untargetable and didn't take any damage" << endl;
	}
}
void Assassin::attack_enemy(Card* &obj1, player &obj2, player &obj3) {
	if (this->is_dead()) {
		cout << "this card is dead and cannot perform desired action!" << endl;
	}
	else {
		if (obj1->state != "untargetable") {
			float temp = 0;
			int store = this->attack;
			if (this->state == "scared") store -= 16;
			if (store < 1) store = 1;
			int hit;
			float chance = (((float)store / (float)obj1->def) * 50);
			if (chance > 50)
			{
				temp = chance - 50;
				temp = sqrt(temp);
				chance = 50 + temp * 3;
			}
			else if (chance < 50)
			{
				temp = 50 - chance;
				temp = sqrt(temp);
				chance = 50 - temp * 3;
			}
			if (chance > 95) chance = 95;
			else if (chance < 5) chance = 5;
			srand(time(0));
			hit = (rand() % 100);
			if (hit <= chance) {
				int tmp = obj1->health;
				obj1->health = obj1->health - this->dmg;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
				if (obj1->health < 0) obj1->health = 0;
				cout << "(Assassin attack) enemy has been hit and his hp therefore reduced from " << tmp << " to " << obj1->health << endl;
			}
			else {
				cout << "enemy unit's armour has sucessfully blocked your card's attack and it's health remains untouched at " << obj1->health << " points, but the attacker is an assassin so it still damages enemy" << endl;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
			}
			if (this->state == "cursed") {
				obj3.hp -= this->dmg;
				if (obj3.hp < 0) obj3.hp = 0;
				cout << "this card is cursed and hurt you! your hp has dropped to: " << obj3.hp << endl;
			}
		}
		else {
			cout << "this unit is untargetable and didn't take any damage" << endl;
		}
	}
}
void Bard::attack_enemy(Card* &obj1, player &obj2, player &obj3) {
	if (this->is_dead()) {
		cout << "this card is dead and cannot perform desired action!" << endl;
	}
	else {
		if (obj1->state != "untargetable") {
			float temp = 0;
			int store = this->attack;
			if (this->state == "scared") store -= 16;
			if (store < 1) store = 1;
			int hit;
			float chance = (((float)store / (float)obj1->def) * 50);
			if (chance > 50)
			{
				temp = chance - 50;
				temp = sqrt(temp);
				chance = 50 + temp * 3;
			}
			else if (chance < 50)
			{
				temp = 50 - chance;
				temp = sqrt(temp);
				chance = 50 - temp * 3;
			}
			if (chance > 95) chance = 95;
			else if (chance < 5) chance = 5;
			srand(time(0));
			hit = (rand() % 100);
			if (hit <= chance) {
				int tmp = obj1->health;
				obj1->health = obj1->health - this->dmg;
				obj2.hp -= this->dmg;
				if (obj2.hp < 0) obj2.hp = 0;
				if (obj1->health < 0) obj1->health = 0;
				if (this->state == "cursed") {
					obj3.hp -= this->dmg;
					if (obj3.hp < 0) obj3.hp = 0;
					cout << "this card is cursed and hurt you! your hp has dropped to: " << obj3.hp << endl;
				}
				cout << "(Mage attack) enemy has been hit and his hp therefore reduced from " << tmp << " to " << obj1->health << endl;
			}
			else cout << "enemy unit's armour has sucessfully blocked your card's attack and it's health remains untouched at " << obj1->health << " points" << endl;
		}
		else cout << "this unit is untargetable and didn't take any damage" << endl;
	}
}
void Assassin::special_abilities(Card* &obj1, player &obj2, player &obj3) {
	string action;
	cout << "this card has following speciaal abilities, choose the one you wish it to perform: " << endl;
	cout << "1. assassination - deals thrice the damage to the enemy ignoring the cards in his deck" << endl << "2. fade - card becomes untargetable to attacks" << endl;
	cin >> action;
	if (action == "1") {
		this->assassination(obj2);
	}
	else if (action == "2") {
		this->fade();
	}
	else {
		system("cls");
		cout << "data incorrect, try again: " << endl;
		this->special_abilities(obj1, obj2, obj3);
	}
}
void Mage::special_abilities(Card* &obj1, player &obj2, player &obj3) {
	string action;
	cout << "this card has following speciaal abilities, choose the one you wish it to perform: " << endl;
	cout << "1. fear - reduces enemy card's atatck rate" << endl << "2. iron maiden - all the damage this card does to it's enemy will also be done to it's owner" << endl;
	cin >> action;
	if (action == "1") {
		this->fear(obj1);
	}
	else if (action == "2") {
		this->iron_maiden(obj1);
	}
	else {
		system("cls");
		cout << "data incorrect, try again: " << endl;
		this->special_abilities(obj1, obj2, obj3);
	}
}
void Priest::special_abilities(Card* &obj1, player &obj2, player &obj3) {
	string action;
	cout << "this card has following speciaal abilities, choose the one you wish it to perform: " << endl;
	cout << "1. heal - restores chosen card's hp back to maximum" << endl << "2. purify - dispells all efects from chosen card" << endl;
	cin >> action;
	if (action == "1") {
		this->heal(obj1);
	}
	else if (action == "2") {
		this->purify(obj1);
	}
	else {
		system("cls");
		cout << "data incorrect, try again: " << endl;
		this->special_abilities(obj1, obj2, obj3);
	}
}
void Bard::special_abilities(Card* &obj1, player &obj2, player &obj3) {
	string action;
	cout << "this card has following speciaal abilities, choose the one you wish it to perform: " << endl;
	cout << "1. convert - converts one of enemy's cards to serve you" << endl;
	cin >> action;
	if (action == "1") {
		this->convert(obj2, obj3);
	}
	else {
		system("cls");
		cout << "data incorrect, try again: " << endl;
		this->special_abilities(obj1, obj2, obj3);
	}
}
void Archer::special_abilities(Card* &obj1, player &obj2, player &obj3) {
	string action;
	cout << "this card does not have any special abilities, press any key to countinue: " << endl;
	cin >> action;
}
void Warrior::special_abilities(Card* &obj1, player &obj2, player &obj3) {
	string action;
	cout << "this card does not have any special abilities, press any key to countinue: " << endl;
	cin >> action;
}
void Assassin::assassination(player &obj2) {
	obj2.hp -= 3 * (this->dmg);
}
void Bard::convert(player &obj1, player &obj2) {
	cout << "insert card you want to convert: " << endl;
	int card;
	cin >> card;
	if (obj1.Deck[card] != NULL) {
		if (obj1.Deck[card]->health > 0) {
			if (obj1.Deck[card]->owner == 1) obj1.Deck[card]->owner = 2;
			else if (obj1.Deck[card]->owner == 2) obj1.Deck[card]->owner = 1;
			obj2.deck_size++;
			obj1.deck_size--;
			obj2.Deck[obj2.deck_size] = obj1.Deck[card];
			card++;
			while (obj1.Deck[card] != NULL) {
				obj1.Deck[card - 1] = obj1.Deck[card];
				card++;
			}
			obj1.Deck[card-1] = NULL;
		}
		else {
			char anykey;
			system("cls");
			cout << "this card has already been killed, choose another, press anykey to continue: " << endl;
			cin >> anykey;
			this->convert(obj1, obj2);
		}
	}
	else {
		char anykey;
		system("cls");
		cout << "no such card, press any key to continue: " << endl;
		cin >> anykey;
		this->convert(obj1, obj2);
	}
}
void player::choose_action(turn &obj1, player &obj2, player &obj3) {
	if (this->isAlive()) {
		if (!this->board_flag) {
			this->drawer = thread(&player::callback_function, this, std::ref(obj2));
			this->board_flag = true;
		}
		this->sig();
		string action;
		Sleep(400);
		cout << "choose acttion" << endl << "1. summon card" << endl << "2. end turn" << endl << "3. show deck" << endl << "4. show card detail" << endl << "5. attack enemy card" << endl << "6. attack enemy" << endl << "7. use speial ability" << endl  << "8. upgrade warrior card" << endl;
		cin >> action;
		this->show_stats();
		if (action == "2") this->end_turn(obj1, obj2, obj3);
		else if (action == "1") this->summon_card(obj1, obj2, obj3);
		else if (action == "3") this->show_deck(obj1, obj2, obj3);
		else if (action == "4") this->show_card_detail(obj1, obj2, obj3);
		else if (action == "5") this->attack_enemy_card(obj1, obj2, obj3);
		else if (action == "6") this->attack_enemy(obj1, obj2, obj3);
		else if (action == "7") this->special_ability(obj1, obj2, obj3);
		else if (action == "8") this->upgrade_warrior(obj1, obj2, obj3);
		else {
			cout << "data incrorrect try again" << endl;
			this->show_stats();
			this->choose_action(obj1, obj2, obj3);
		}
	}
	else {
		string anykey;
		cout << "you have lost, player " << obj2.id << " has won, press any key to continue";
		cin >> anykey;
		system("cls");
		menu *men = new menu;
	}
}
