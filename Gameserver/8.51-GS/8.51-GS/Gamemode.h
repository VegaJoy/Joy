#pragma once

#include "Framework.h"
#include "Globals.h"

namespace Gamemode
{
	bool (*ReadyToStartMatch)(AFortGameModeAthena* Game);
	bool ReadyToStartMatchHook(AFortGameModeAthena* Game)
	{
		if (!Helpers::GetGameState()->MapInfo)
			return false;

		if (!Globals::bListening)
		{
			Globals::bListening = true;
			Server::Listen();
			Game->bWorldIsReady = true;
		}

		return true;
	}

	APawn* SpawnDefaultPawnForHook(AGameModeBase* Game, AController* NewPlayer, AActor* StartSpot)
	{
		auto Transform = StartSpot->GetTransform();

		return Game->SpawnDefaultPawnAtTransform(NewPlayer, Transform);
	}

	void GameHooks()
	{
		MH_CreateHook((LPVOID)Helpers::GetOffset(0x307C130), ReadyToStartMatchHook, (void**) & ReadyToStartMatch);
		MH_EnableHook((LPVOID)Helpers::GetOffset(0x307C130));

		MH_CreateHook((LPVOID)Helpers::GetOffset(0xFB4A00), SpawnDefaultPawnForHook, nullptr);
		MH_EnableHook((LPVOID)Helpers::GetOffset(0xFB4A00));
	}
}