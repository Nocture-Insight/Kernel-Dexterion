#pragma once

#include <windows.h>
#include <string>
#include <iostream>

inline class _Logger {
public:
	HANDLE hConsole;

	void Logger(HANDLE hConsole) {
		this->hConsole = hConsole;
	}

	std::wstring StrToWstr(std::string str)
	{
		std::wstring temp;
		std::copy(str.begin(), str.end(), std::back_inserter(temp));
		return temp;
	}

	// WString
	void Info(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 9);
		if (endLine)
			std::wcout << "[Info] " << str << std::endl;
		else
			std::wcout << "[Info] " << str;
	}

	void Success(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 10);
		if (endLine)
			std::wcout << "[Success] " << str << std::endl;
		else
			std::wcout << "[Success] " << str;
	}

	void Error(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 12);
		if (endLine)
			std::wcout << "[Error] " << str << std::endl;
		else
			std::wcout << "[Error] " << str;
	}

	void Warn(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 14);
		if (endLine)
			std::wcout << "[Warning] " << str << std::endl;
		else
			std::wcout << "[Warning] " << str;
	}

	void Log(std::wstring str, bool endLine = true) {
		Info(str, endLine);
	}






	// String
	void Info(std::string str, bool endLine = true) {
		Info(StrToWstr(str), endLine);
	}

	void Success(std::string str, bool endLine = true) {
		Success(StrToWstr(str), endLine);
	}

	void Error(std::string str, bool endLine = true) {
		Error(StrToWstr(str), endLine);
	}

	void Warn(std::string str, bool endLine = true) {
		Warn(StrToWstr(str), endLine);
	}

	void Log(std::string str, bool endLine = true) {
		Log(StrToWstr(str), endLine);
	}
} Logger(GetStdHandle(STD_OUTPUT_HANDLE));

inline namespace utils {
	inline std::string version = "1.0.0";

	inline std::wstring getExePath() {
		return std::wstring(L"C:\\Dexterion");
	}

	inline std::wstring getConfigPath() {
		return std::wstring(L"C:\\Dexterion\\Config");
	}
}