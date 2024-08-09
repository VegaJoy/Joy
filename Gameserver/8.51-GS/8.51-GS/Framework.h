#pragma once

#include <Windows.h>
#include <iostream>
#include <string>

#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/minhook.lib")

#include "SDK/SDK.hpp";
using namespace SDK;

#define LOG(...) std::cout << "LOG: " << __VA_ARGS__ << "\n";

namespace Helpers
{
	inline uintptr_t GetOffset(uintptr_t Offset)
	{
		return __int64(GetModuleHandleW(nullptr)) + Offset;
	}

	UWorld* GetWorld()
	{
		return UObject::FindObject<UFortEngine>("FortEngine_")->GameViewport->World;
	}

	AFortGameStateAthena* GetGameState()
	{
		return (AFortGameStateAthena*)GetWorld()->GameState;
	}

	AFortGameModeAthena* GetGameMode()
	{
		return (AFortGameModeAthena*)GetWorld()->AuthorityGameMode;
	}

	template<typename T>
	T* GetDefObj()
	{
		return (T*)T::StaticClass()->DefaultObject;
	}
}

namespace Server
{
	static UNetDriver* (*CreateNetDriver)(UEngine*, UWorld*, FName) = decltype(CreateNetDriver)(Helpers::GetOffset(0x2FBED30));
	static bool (*InitListen)(void*, void*, FURL&, bool, FString&) = decltype(InitListen)(Helpers::GetOffset(0x634C10));
	static void (*SetWorld)(UNetDriver*, UWorld*) = decltype(SetWorld)(Helpers::GetOffset(0x2D38590));
	static void (*ServerReplicateActors)(void*);

	void Listen()
	{
		Helpers::GetWorld()->NetDriver = CreateNetDriver(UObject::FindObject<UFortEngine>("FortEngine_"), Helpers::GetWorld(), FName(282));

		Helpers::GetWorld()->NetDriver->NetDriverName = FName(282);
		Helpers::GetWorld()->NetDriver->World = Helpers::GetWorld();

		FString err;
		auto URL = FURL();
		URL.Port = 7777;

		InitListen(Helpers::GetWorld()->NetDriver, Helpers::GetWorld(), URL, false, err);
		SetWorld(Helpers::GetWorld()->NetDriver, Helpers::GetWorld());

		Helpers::GetWorld()->LevelCollections[0].NetDriver = Helpers::GetWorld()->NetDriver;
		Helpers::GetWorld()->LevelCollections[1].NetDriver = Helpers::GetWorld()->NetDriver;

		void** ReplicationDriverVTable = *(void***)Helpers::GetWorld()->NetDriver->ReplicationDriver;
		ServerReplicateActors = decltype(ServerReplicateActors)(ReplicationDriverVTable[0x56]);

		LOG("Listening On Port " << URL.Port);
	}
}