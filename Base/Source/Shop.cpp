#include "Shop.h"

Shop::Shop()
:m_playerMoney(0)
, theItemHolder(NULL)
, ShopItem(NULL)
, itemholder(NULL)
{

}
Shop::~Shop()
{
	if (ShopItem != NULL)
	{
		delete ShopItem;
		ShopItem = NULL;
	}
	for (vector<Item*>::iterator it = theItemHolder.begin(); it != theItemHolder.end(); ++it)
	{
		Item* item = (Item*)*it;

		if (item != NULL)
		{
			delete item;
			item = NULL;
		}
	}
}
void Shop::PlayerInit(Player* playerinfo)
{
	m_playerMoney = playerinfo->GetAmtOfCurrency();
}
void Shop::ItemInit()
{
	ShopItem = new LuaUsage();
	ShopItem->LuaUsageInit("Item");
	int sizeofvector = ShopItem->get<int>("Amount");
	for (int i = 0; i < sizeofvector; ++i)
	{
		itemholder = new Item();
		string luaitemnaming = "Item.Number";
		luaitemnaming = luaitemnaming + to_string((i+1));
		string luaitemname = luaitemnaming + ".Name";
		string luaitemprice = luaitemnaming + ".Price";
		string luaitembought = luaitemnaming + ".Bought";
		string itemname = ShopItem->get<string>(luaitemname);
		bool itembought = ShopItem->get<bool>(luaitembought);
		int itemprice = ShopItem->get<int>(luaitemprice);
		itemholder->Init(itemname,itembought,itemprice);
		theItemHolder.push_back(itemholder);
	}
	ShopItem->LuaUsageClose();
}
void Shop::Set(Player* playerinfo)
{
	playerinfo->SetAmtOfCurrency(m_playerMoney);
}
void Shop::Buy(int selected)
{
	if (selected < theItemHolder.size())
	{
		m_playerMoney = m_playerMoney - theItemHolder[selected]->GetPrice();
		theItemHolder[selected]->SetBought(true);
	}
	
}