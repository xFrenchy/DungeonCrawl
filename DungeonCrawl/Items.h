#pragma once
#ifndef ITEMS_H
#define ITEMS_H
#include <string>

using std::string;

//TODO create a class or struct for objects to make a better inventory since the inventory is strings
//int treasuresGold[] = { 25, 50, 100, 250, 500, 1000 };	//6 items
//std::string treasureStatBoost[] = { "+1 Attack", "+3 Attack", "+5 Attack", "+1 Strength", "+3 Strength", "+5 Strength", "+1 Defence", "+3 Defence", "+5 Defence" };	//9 items
//std::string treasureFood[] = { "Fish", "Lobster", "Minion Meat", "Shark", "Dragon Meat" };	//5 items

//wrap all the consumable in a class that way inventory will be a vector of that class
//use items will take in an item of type consumable that holds all the struct

struct Item
{
     string name;
     int HealValue;
     int amount;
     bool consumable;    //might be useless to have this variable
};

/*
Fish.name = "Fish Meat";
Fish.HealValue = 5;
Fish.consumable = true;
Fish.amount = 1;
MinionMeat.name = "Minion Meat";
MinionMeat.HealValue = 10;
MinionMeat.consumable = true;
MinionMeat.amount = 1;
SmallDragonMeat.name = "Small Dragon Meat";
SmallDragonMeat.HealValue = 15;
SmallDragonMeat.consumable = true;
SmallDragonMeat.amount = 1;
Shark.name = "Shark Meat";
Shark.HealValue = 20;
Shark.consumable = true;
Shark.amount = 1;
DragonMeat.name = "Dragon Meat";
DragonMeat.HealValue = 25;
DragonMeat.consumable = true;
DragonMeat.amount = 1;
*/
#endif //items_h