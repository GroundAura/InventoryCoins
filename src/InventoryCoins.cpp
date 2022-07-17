#include "InventoryCoins.h"
#include "DataHandler.h"

namespace Coins
{
	float get_coins_weight()
	{
		return DataHandler::Gold001->GetWeight() *
		       FenixUtils::get_item_count(RE::PlayerCharacter::GetSingleton(), DataHandler::Gold001);
	}

	float get_purse_weight(RE::TESObjectREFR* item) { return DataHandler::Gold001->GetWeight() * item->extraList.GetCount(); }
}
