#include <Windows.h>
#include <TlHelp32.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

namespace DriverCodes {
	constexpr ULONG Attach = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	constexpr ULONG Read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	constexpr ULONG Write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
}

struct DriverRequest {
	HANDLE process_id;

	PVOID target;
	PVOID buffer;

	SIZE_T size;
	SIZE_T return_size;
};

class MemoryManagement {
public:
	DWORD GetPid(const wchar_t* process_name) {
		DWORD process_id = 0;

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE)
			return process_id;

		PROCESSENTRY32W entry = {};
		entry.dwSize = sizeof(decltype(entry));

		if (Process32FirstW(snapshot, &entry) == TRUE) {
			if (_wcsicmp(process_name, entry.szExeFile) == 0)
				process_id = entry.th32ProcessID;
			else {
				while (Process32NextW(snapshot, &entry) == TRUE) {
					if (_wcsicmp(process_name, entry.szExeFile) == 0) {
						process_id = entry.th32ProcessID;
						break;
					}
				}
			}
		}

		CloseHandle(snapshot);

		return process_id;
	}

	uintptr_t GetModuleBase(const DWORD pid, const wchar_t* module_name) {
		uintptr_t module_base = 0;

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
		if (snapshot == INVALID_HANDLE_VALUE)
			return module_base;

		MODULEENTRY32W entry = {};
		entry.dwSize = sizeof(decltype(entry));

		if (Module32FirstW(snapshot, &entry) == TRUE) {
			if (wcsstr(module_name, entry.szModule) != nullptr)
				module_base = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
			else {
				while (Module32NextW(snapshot, &entry) == TRUE) {
					if (wcsstr(module_name, entry.szModule) != nullptr) {
						module_base = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
						break;
					}
				}
			}
		}

		CloseHandle(snapshot);

		return module_base;
	}

	bool AttachToProccess(HANDLE handle, const DWORD pid) {
		DriverRequest r;
		r.process_id = reinterpret_cast<HANDLE>(pid);

		return DeviceIoControl(handle, DriverCodes::Attach, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
	}

	template <class T>
	T Read(HANDLE handle, const uintptr_t addr) {
		T temp = {};

		DriverRequest r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = &temp;
		r.size = sizeof(T);

		DeviceIoControl(handle, DriverCodes::Read, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);

		return temp;
	}

	template <class T>
	void Write(HANDLE handle, uintptr_t addr, const T& value) {
		DriverRequest r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = (PVOID)&value;
		r.size = sizeof(T);

		DeviceIoControl(handle, DriverCodes::Write, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
	}
};

MemoryManagement Driver;