#pragma once

class DataHandler
{
	static inline std::string_view esp_name = "ThrowIT.esp"sv;

public:
	static inline RE::TESObjectMISC* Gold001 = nullptr;
	static inline RE::TESObjectMISC* f314IC_CoinPurse = nullptr;

	static void init()
	{
		auto datahandler = RE::TESDataHandler::GetSingleton();

		Gold001 = RE::TESForm::LookupByID<RE::TESObjectMISC>(0xf);
		f314IC_CoinPurse = datahandler->LookupForm<RE::TESObjectMISC>(0x800, esp_name);
	}
};
