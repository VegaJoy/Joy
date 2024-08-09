#include "Framework.h"
#include "Network.h"
#include "Gamemode.h"

DWORD Main(LPVOID)
{
	AllocConsole();
	FILE* streamPtr;
	freopen_s(&streamPtr, "CONOUT$", "w", stdout);
	SetConsoleTitleA("8.51-GS");

    MH_Initialize();

    InitGObjects();

    Sleep(5000);

    MH_CreateHook((LPVOID)Helpers::GetOffset(0x2D39300), Network::TickFlushHook, (void**) & Network::TickFlush);
    MH_EnableHook((LPVOID)Helpers::GetOffset(0x2D39300));

    MH_CreateHook((LPVOID)Helpers::GetOffset(0x15392D0), Network::ValidationFailure, nullptr);
    MH_EnableHook((LPVOID)Helpers::GetOffset(0x15392D0));

    MH_CreateHook((LPVOID)Helpers::GetOffset(0x1542210), Network::NoReservation, nullptr);
    MH_EnableHook((LPVOID)Helpers::GetOffset(0x1542210));

    ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->ExecuteConsoleCommand(Helpers::GetWorld(), L"open Athena_Terrain", nullptr);
    UObject::FindObject<UFortEngine>("FortEngine_")->GameInstance->LocalPlayers.Remove(0);

    Gamemode::GameHooks();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}