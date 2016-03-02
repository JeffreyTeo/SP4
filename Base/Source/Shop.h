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
	void BuyItem(int selected);
	void Set(Player* playerinfo);
	int GetPlayerMoney();
	int GetPlayerGold();
	bool GetplayerEasyModeUnlocked();
	bool GetplayerNormalModeUnlocked();
	bool GetplayerHardModeUnlocked();
	vector<Item*> theItemHolder;
private:
	Item* itemholder;
	LuaUsage* ShopItem;
	int m_playerMoney;
	int m_playerGold;
	bool m_playerEasyModeUnlocked;
	bool m_playerNormalModeUnlocked;
	bool m_playerHardModeUnlocked;
};
#endif