#define WIN32_LEAN_AND_MEAN 
#include <iostream>
#include <thread>

#include "offsets/offsets.cpp"
#include "gui/overlay.hpp"
#include "features/entry.hpp"

LRESULT Wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return 0;

	if (msg == WM_SYSCOMMAND)
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;

	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	const HANDLE driver = CreateFileW(L"\\\\.\\DexterionDriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (driver == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create driver!" << std::endl;
		std::cin.get();
		return 1;
	}

#ifndef DEBUG_MSEDGE

	const DWORD pid = Driver.GetPid(L"cs2.exe");

	if (pid == 0) {
		std::cout << "Failed to get Counter-Strike 2!" << std::endl;
		std::cin.get();
		return 1;
	}

	MemoryManagement::moduleData client;

	loadJson();

	if (Driver.AttachToProccess(driver, pid) == true) {
		std::cout << "Attachment successful!" << std::endl;

		client.base = Driver.GetModuleBase(pid, L"client.dll");
		if (client.base != 0) {
			std::cout << "Client found!" << std::endl;

			config::refresh();
			config::load(0);

			bool state = true;

			Gui gui;
			WNDCLASSEXW winClass = gui.CreateWindowClass(hInstance, Wndproc, L"Dexterion");
			HWND window = gui.CreateOWindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

			gui.MakeFrameIntoClientArea();
			gui.MakeDeviceAndSwapChain();
			gui.InitWindow(nShowCmd);

			while (state) {
				if (GetKeyState(VK_END))
					state = false;

				gui.RenderLoop();

				mainLoop(state, client);

				gui.EndRenderLoop();
			}

			gui.DestroyOWindow();
		}
	}
#else
	const DWORD pid = Driver.GetPid(L"msedge.exe");

	if (pid == 0) {
		std::cout << "Failed to get msedge.exe!" << std::endl;
		std::cin.get();
		return 1;
	}

	MemoryManagement::moduleData client;

	uint64_t a[3] = { 0xFD4C760, 0x0, 0x0 };

	if (Driver.AttachToProccess(driver, pid) == true) {
		std::cout << "Attachment successful!" << std::endl;

		client.base = Driver.GetModuleBase(pid, L"msedge.dll");

		while (client.base != 0) {
			std::cout << "Tabs Open: " << Driver.Read<int>(client.base + a[0]) << std::endl;
		}
	}
#endif

	CloseHandle(driver);

	std::cin.get();

	return 0;
}