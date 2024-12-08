#include "Utils/Utils.h"
#include "Utils/Backend/Backend.h"
#include "Utils/Hooks/Hooks.h"
#include <iostream>
#include <intrin.h>
#include "Utils/BNM-IL2CPP/BNM.hpp"
#include <cstdint>
#include <string>

#define QWORD int64_t

// Run it in Release | x64, already set up everything

QWORD WINAPI MainThread(LPVOID param)
{
	IL2CPP::Initialize();
	RunBackend.Load(); // Load everything
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);;
	SetConsoleTitleA("PG3D Internal");
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	Hooks();
	printf("Press Right Control to open the menu.\n");

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(420)); // No cpu frying in my city
		if (Utils::keyPressed(UNINJECT_KEY)) break; // Uninject
	}
	
	RunBackend.Unload(); // Unload everything
	return 0;
}

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case 1:
		HANDLE hMainThread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
		if (hMainThread)
			CloseHandle(hMainThread);
		break;
	}

	return TRUE;
}