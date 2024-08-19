#pragma once

#include <windows.h>
#include <string>
#include <iostream>

#include "../../lib/imgui/imgui.h"

inline class _Logger {
public:
	HANDLE hConsole;

	void Logger(HANDLE hConsole) {
		this->hConsole = hConsole;
	}

	std::string WStrTostr(std::wstring str)
	{
		return std::string(str.begin(), str.end());
	}

	// String
	void Info(std::string str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 9);
		if (endLine)
			std::cout << "[Info] " << str << std::endl;
		else
			std::cout << "[Info] " << str;
	}

	void Success(std::string str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 10);
		if (endLine)
			std::cout << "[Success] " << str << std::endl;
		else
			std::cout << "[Success] " << str;
	}

	void Error(std::string str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 12);
		if (endLine)
			std::cout << "[Error] " << str << std::endl;
		else
			std::cout << "[Error] " << str;
	}

	void Warn(std::string str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 14);
		if (endLine)
			std::cout << "[Warning] " << str << std::endl;
		else
			std::cout << "[Warning] " << str;
	}

	void Log(std::string str, bool endLine = true) {
		Info(str, endLine);
	}






	// WString
	void Info(std::wstring str, bool endLine = true) {
		Info(WStrTostr(str), endLine);
	}

	void Success(std::wstring str, bool endLine = true) {
		Success(WStrTostr(str), endLine);
	}

	void Error(std::wstring str, bool endLine = true) {
		Error(WStrTostr(str), endLine);
	}

	void Warn(std::wstring str, bool endLine = true) {
		Warn(WStrTostr(str), endLine);
	}

	void Log(std::wstring str, bool endLine = true) {
		Log(WStrTostr(str), endLine);
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

	inline bool IsGameWindowActive() {
		HWND hwnd = GetForegroundWindow();
		if (hwnd) {
			char windowTitle[256];
			GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
			return std::string(windowTitle).find("Counter-Strike 2") != std::string::npos;
		}
		return false;
	}

	inline ImColor Float3ToImColor(float colors[3], float a = 1.f) {
		return ImColor(colors[0], colors[1], colors[2], a);
	}

	inline ImColor Float4ToImColor(float colors[4]) {
		return ImColor(colors[0], colors[1], colors[2], colors[3]);
	}
}