#pragma once
#pragma warning (disable: 4302)
#pragma warning (disable: 4129)
#pragma warning (disable: 4244)
#pragma warning (disable: 6031)

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <Windows.h>

#include "../imgui/imgui.h"
#include "Vectors.h"

#include <comdef.h>
#include <Wbemidl.h>

inline namespace Logger {
	inline HANDLE hConsole;

	inline std::wstring StrToWstr(std::string str)
	{
		std::wstring temp;
		std::copy(str.begin(), str.end(), std::back_inserter(temp));
		return temp;
	}

	// WString
	inline void info(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 9);
		if (endLine)
			std::wcout << "[Info] " << str << std::endl;
		else
			std::wcout << "[Info] " << str;
	}

	inline void success(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 10);
		if (endLine)
			std::wcout << "[Success] " << str << std::endl;
		else
			std::wcout << "[Success] " << str;
	}

	inline void error(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 12);
		if (endLine)
			std::wcout << "[Error] " << str << std::endl;
		else
			std::wcout << "[Error] " << str;
	}

	inline void warn(std::wstring str, bool endLine = true) {
		SetConsoleTextAttribute(hConsole, 14);
		if (endLine)
			std::wcout << "[Warning] " << str << std::endl;
		else
			std::wcout << "[Warning] " << str;
	}






	// String
	inline void info(std::string str, bool endLine = true) {
		info(StrToWstr(str), endLine);
	}

	inline void success(std::string str, bool endLine = true) {
		success(StrToWstr(str), endLine);
	}

	inline void error(std::string str, bool endLine = true) {
		error(StrToWstr(str), endLine);
	}

	inline void warn(std::string str, bool endLine = true) {
		warn(StrToWstr(str), endLine);
	}
}

inline namespace utils {
	inline std::string version = "2.2.1";

	// https://www.unknowncheats.me/forum/dayz-sa/129893-calculate-distance-meters.html
	// https://www.unknowncheats.me/forum/general-programming-and-reversing/478087-calculate-size-esp-boxes-based-distance.html
	inline float getDistance(Vector3 from, Vector3 to) {
		return sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2) + pow(to.z - from.z, 2));
	};

	inline std::string get_hwid() {
		HW_PROFILE_INFO hwProfileInfo;
		if (GetCurrentHwProfile(&hwProfileInfo))
			return hwProfileInfo.szHwProfileGuid;
	}

	inline std::wstring getExePath() {
		return std::wstring(L"C:\\Dexterion");
	}

	inline std::wstring getConfigPath() {
		return std::wstring(L"C:\\Dexterion\\Config");
	}

	inline ImColor float3ToImColor(float colours[3], float a = 1.f) {
		return ImColor(colours[0], colours[1], colours[2], a);
	}

	inline uint64_t currentTimeMillis() {
		using namespace std::chrono;
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}

	inline bool intToBool(int i) {
		return i != 0;
	}

	inline namespace espF {
		inline float fixFontSize(float size) {
			int returnSize = 1;

			if (size > 4.f) returnSize = 4.f;
			if (size < 1.f) returnSize = 1.f;

			return returnSize;
		}

		// This is with size being 5 !!!
		inline float fixJointSize(float size) {
			int returnSize = 1;

			if (size > 3.f) returnSize = 3.f;
			if (size > 7.f) returnSize = 4.f;
			if (size < 1.f) returnSize = 2.f;

			return returnSize;
		}

		inline float getFontSize(float fontSize,int distance) {
			return (fontSize - utils::fixFontSize(distance));
		}

		inline float getJointSize(float joinSize, int distance) {
			return (joinSize - utils::fixJointSize(distance));
		}

		inline std::tuple<float,float> getTextOffsets(float x,float y, float horizontalDivide,float verticalDivide = 1) {
			float horizontalOffset = x / horizontalDivide;
			float verticalOffset = y - verticalDivide;

			std::tuple<float,float> coords = { horizontalOffset, verticalOffset };
			return coords;
		}
	}
}