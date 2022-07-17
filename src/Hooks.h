#pragma once

#include "InventoryCoins.h"

class UICardHook
{
public:
	static void Hook()
	{
		_get_weight_inv =
			SKSE::GetTrampoline().write_call<5>(REL::ID(51019).address() + 0x17c, get_weight_inv);  // SkyrimSE.exe+890FEC
		_get_weight_hud =
			SKSE::GetTrampoline().write_call<5>(REL::ID(50718).address() + 0x316, get_weight_hud);      // SkyrimSE.exe+87D896
		_get_value = SKSE::GetTrampoline().write_call<5>(REL::ID(50718).address() + 0x363, get_value);  // SkyrimSE.exe+87D8E3
	}

private:
	static float get_weight_inv(RE::TESBoundObject* item)
	{
		if (item->formID == 0xf) {
			return Coins::get_coins_weight();
		} else {
			return _get_weight_inv(item);
		}
	}

	static float get_weight_hud(RE::TESObjectREFR* item)
	{
		if (item->GetBaseObject()->formID == DataHandler::f314IC_CoinPurse->formID) {
			return Coins::get_purse_weight(item);
		} else {
			return _get_weight_hud(item);
		}
	}

	static uint32_t get_value(RE::InventoryEntryData* invdata) {
		if (invdata->object->formID == DataHandler::f314IC_CoinPurse->formID) {
			uint32_t count = 0;
			if (auto lists = invdata->extraLists; lists->begin() != lists->end() && *lists->begin()) {
				auto list = *lists->begin();
				count = list->GetCount();
			}
			return DataHandler::Gold001->GetGoldValue() * count;
		} else {
			return _get_value(invdata);
		}
	}

	static inline REL::Relocation<decltype(get_weight_inv)> _get_weight_inv;
	static inline REL::Relocation<decltype(get_weight_hud)> _get_weight_hud;
	static inline REL::Relocation<decltype(get_value)> _get_value;
};

class IsGoldHook
{
public:
	static void Hook()
	{
		_IsGold = SKSE::GetTrampoline().write_call<5>(REL::ID(50941).address() + 0x105, IsGold);  // SkyrimSE.exe+88B4F5
	}

private:
	static bool IsGold(RE::TESForm*) { return false; }

	static inline REL::Relocation<decltype(IsGold)> _IsGold;
};

class PlayerTakeDropHook
{
public:
	static void Hook()
	{
		_DropObject = REL::Relocation<uintptr_t>(REL::ID(RE::VTABLE_PlayerCharacter[0])).write_vfunc(0xcb, DropObject);
		_sub_14028D880 =
			SKSE::GetTrampoline().write_call<5>(REL::ID(39456).address() + 0x4b7, sub_14028D880);  // SkyrimSE.exe+6A57B7
	}

private:
	static uint32_t* DropObject(RE::PlayerCharacter* a, uint32_t* handle_ans, RE::TESBoundObject* _item,
		RE::ExtraDataList* extraList, uint32_t count, void* arg28, void* a7)
	{
		if (_item->formID == 0xf) {
			FenixUtils::AddItemPlayer(DataHandler::f314IC_CoinPurse, count);
			FenixUtils::RemoveItemPlayer(DataHandler::Gold001, count);
			return _DropObject(a, handle_ans, DataHandler::f314IC_CoinPurse, extraList, count, arg28, a7);
		} else {
			return _DropObject(a, handle_ans, _item, extraList, count, arg28, a7);
		}
	}

	static void sub_14028D880(RE::TESObjectREFR* a, RE::TESObjectREFR* item, uint32_t count, char a4, char a5) {
		if (item->GetBaseObject()->formID == DataHandler::f314IC_CoinPurse->formID) {
			item->data.objectReference = DataHandler::Gold001;
			return _sub_14028D880(a, item, count, a4, a5);
		} else {
			return _sub_14028D880(a, item, count, a4, a5);
		}
	}

	static inline REL::Relocation<decltype(DropObject)> _DropObject;
	static inline REL::Relocation<decltype(sub_14028D880)> _sub_14028D880;
};
