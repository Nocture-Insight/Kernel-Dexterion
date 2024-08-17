#include <iostream>
#include "util/Memory.hpp"
#include "offsets/include.hpp"

int main() {
	const HANDLE driver = CreateFile(L"\\\\.\\DexterionDriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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

			// BHop for testing

			while (true) {
				if (GetAsyncKeyState(VK_END))
					break;

				const uintptr_t localPlayerPawn = Driver.Read<uintptr_t>(client.base + offsets::clientDLL["dwLocalPlayerPawn"]);

				if (localPlayerPawn == 0)
					continue;

				const auto flags = Driver.Read<uint32_t>(localPlayerPawn + clientDLL::C_BaseEntity_["m_fFlags"]);

				const bool in_air = flags & (1 << 0);
				const bool space_pressed = GetAsyncKeyState(VK_SPACE);

				if (space_pressed && in_air) {
					Sleep(5);
					Driver.Write(client.base + buttons["jump"], 65537);
				}
				else if (space_pressed && !in_air) {
					Driver.Write(client.base + buttons["jump"], 256);
				}
			}
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