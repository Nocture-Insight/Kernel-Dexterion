#pragma once

#include <fstream>

#include "json.hpp"

inline namespace json {
	inline std::wstring clientDLLFile = L"\\client.dll.json";
	inline std::wstring offsetFile = L"\\offsets.json";
	inline std::wstring buttonsFile = L"\\buttons.json";

	inline nlohmann::json readFromJsonFile(std::wstring path, std::wstring fileName) {
		nlohmann::json fileContent;
		try {
			std::wstring currentPath;
			std::wstring Directory = path;
			Directory = Directory + L"\\";
			currentPath = Directory.append(fileName);

			std::ifstream f(currentPath);
			fileContent = nlohmann::json::parse(f);

			f.close();
		}
		catch ([[maybe_unused]] nlohmann::json::parse_error& ex) {
			return 0;
		}
		catch ([[maybe_unused]] nlohmann::json::type_error& ex) {
			return 0;
		}
		return fileContent;
	}
}