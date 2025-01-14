#include "Cheat/Utils/Utils.h"
#include "Cheat/Backend/Backend.h"
#include "Cheat/Game/Hooks.h"

#define QWORD int64_t

// Run it in Release | x64, already set up everything

QWORD WINAPI MainThread(LPVOID param)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);;
	SetConsoleTitleA("PG3D Internal");
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	IL2CPP::Initialize();
	RunBackend.Load(); // Load everything

	Hooks();
	printf("Press Insert to open the menu.\n");
	
	// Log tests
	/*
	Logger::Log("This is an info message.", Logger::LogLevel::INFO);
	Logger::Log("This is a warning message.", Logger::LogLevel::WARNING);
	Logger::Log("This is an error message. {}", Logger::LogLevel::ERRORR, "fiken!!!");
	Logger::Log("This is a success message.", Logger::LogLevel::SUCCESS);
	*/

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(420));
		if (Utils::keyPressed(UNINJECT_KEY)) break; // Uninject
	}
	
	RunBackend.Unload();
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