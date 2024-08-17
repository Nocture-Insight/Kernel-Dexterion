#include <iostream>
#include "util/Memory.hpp"

int main() {
	const DWORD pid = Driver.GetPid(L"notepad.exe");

	if (pid == 0) {
		std::cout << "Failed to get notepad!" << std::endl;
		std::cin.get();
		return 1;
	}

	const HANDLE driver = CreateFile(L"\\\\.\\DexterionDriver", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (driver == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create driver!" << std::endl;
		std::cin.get();
		return 1;
	}

	if (Driver.AttachToProccess(driver, pid) == true)
		std::cout << "Attachment successful!" << std::endl;

	CloseHandle(driver);

	std::cin.get();

	return 0;
}