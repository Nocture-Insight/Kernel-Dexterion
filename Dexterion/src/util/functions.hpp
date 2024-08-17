#pragma once
#include <string>
#include <windows.h>

inline namespace utils {
	inline std::string version = "1.0.0";

	std::wstring getExePath() {
		return std::wstring(L"C:\\Dexterion");
	}

	std::wstring getConfigPath() {
		return std::wstring(L"C:\\Dexterion\\Config");
	}
}