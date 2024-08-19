#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <thread>
#include <chrono>
#include <format>

#include "util/config.hpp"
#include "gui/overlay.hpp"
#include "util/MemMan.hpp"
#include "util/attributes.hpp"

LRESULT Wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return 0;
	}
	switch (msg) {
		case WM_SYSCOMMAND: {
			if ((wParam & 0xfff0) == SC_KEYMENU)
				return 0;
			break;
		}

		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	const HANDLE driver = CreateFileW(L"\\\\.\\DexterionDriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (driver == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create driver!" << std::endl;
		std::cin.get();
		return 1;
	}

	Logger::hConsole = hConsole;

	// Memory and game related vars (used in entry and passed through overlay)
	int procId = MemMan.GetPid(L"cs2.exe");
	// Weird method until I find a proper fix, im tired rn
	if (procId == 0) {
		Logger::info("[MemMan] Waiting For Counter Strike 2");
		while (procId == 0) {
			procId = MemMan.GetPid(L"cs2.exe");
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}
	}
	Logger::success("[MemMan] Counter Strike 2 Found!");
	MemMan.AttachToProccess(driver, procId);
	Logger::info("[Config.hpp] Checking for config file...");
	config::refresh();
	if (config::exists(0)) { // passing 0 cause setup
		Logger::success("[Config.hpp] Config File Found! Loading config...");
		config::load(0);
	}
	else {
		Logger::error("[Config.hpp] Config File Not Found! Loading Defaults...");
		config::create(L"config.json");
		config::save(0);
	}
	
	Logger::info("[dexterion.cpp] Getting addresses...");
	MemoryManagement::moduleData client;
	client.module = 0;
	client.base = MemMan.GetModuleBase(procId, L"client.dll");
	while (client.base == 0) {
		client.base = MemMan.GetModuleBase(procId, L"client.dll");
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	}
	if (!loadJson()) {
		Logger::error("[attributes.cpp] Cannot load JSON files (did you run updateoffsets.cmd?)");
		system("pause");
		return 0;
	}
	Logger::success("[dexterion.cpp] Addresses found succesfully!");
	
	Logger::info("[dexterion.cpp] Creating overlay...");
	// Overlay
	overlayESP overlayClass;
	WNDCLASSEXW windowClass = overlayClass.createWindowClass(hInstance, Wndproc, L"Dexterion");
	HWND window = overlayClass.createWindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	Logger::info("[dexterion.cpp] Drawing overlay...");
	overlayClass.makeFrameIntoClientArea();
	overlayClass.makeDeviceAndSwapChain();
	overlayClass.initWindow(nShowCmd);

	Logger::info("[overlay.cpp] Starting main loop...");
	overlayClass.renderLoop(client);

	return 0;
}