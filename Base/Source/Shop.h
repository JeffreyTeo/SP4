#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include <string>
#include "LuaUsage.h"
#include "Player.h"
#include "Item.h"

using namespace std;

class Shop
{
public:
	Shop();
	~Shop();
	void PlayerInit(Player* playerinfo);
	void ItemInit();
	void Buy(int selected);
	void Set(Player* playerinfo);
	vector<Item*> theItemHolder;
private:
	Item* itemholder;
	LuaUsage* ShopItem;
	int m_playerMoney;
	
};
#endif