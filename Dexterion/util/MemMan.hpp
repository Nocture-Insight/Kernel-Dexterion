#pragma once
#pragma warning(disable: 4312)

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <winioctl.h>

#define BYTE_InRange(x, a, b)	(x >= a && x <= b) 
#define BYTE_GetBits(x)			(BYTE_InRange((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (BYTE_InRange(x,'0','9') ? x - '0' : 0))
#define BYTE_Get(x)				(BYTE_GetBits(x[0]) << 4 | BYTE_GetBits(x[1]))

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
	HANDLE driv;

	struct moduleData {
		HMODULE module;
		DWORD_PTR base;
		uintptr_t size;
	};

	__forceinline bool Signature_IsMatch(uint8_t* m_Address, uint8_t* m_Pattern, uint8_t* m_Mask)
	{
		size_t m_Index = 0;
		while (m_Address[m_Index] == m_Pattern[m_Index] || m_Mask[m_Index] == (uint8_t)('?'))
		{
			if (!m_Mask[++m_Index])
				return true;
		}

		return false;
	}

	uintptr_t FindSignature(uintptr_t m_Address, uintptr_t m_Size, const char* m_Signature)
	{
		size_t m_SignatureLength = strlen(m_Signature);

		uint8_t* m_PatternAlloc = new uint8_t[m_SignatureLength >> 1];
		uint8_t* m_MaskAlloc = new uint8_t[m_SignatureLength >> 1];
		uint8_t* m_Pattern = m_PatternAlloc;
		uint8_t* m_Mask = m_MaskAlloc;

		// Run-Time IDA Sig to Sig & Mask
		size_t m_PatternLength = 0;
		while (*m_Signature)
		{
			if (*m_Signature == ' ') m_Signature++;
			if (!*m_Signature) break;

			if (*(uint8_t*)(m_Signature) == (uint8_t)('\?'))
			{
				*m_Pattern++ = 0;
				*m_Mask++ = '?';
				m_Signature += ((*(uint16_t*)(m_Signature) == (uint16_t)('\?\?')) ? 2 : 1);
			}
			else
			{
				*m_Pattern++ = BYTE_Get(m_Signature);
				*m_Mask++ = 'x';
				m_Signature += 2;
			}

			++m_PatternLength;
		}

		// Find Address
		*m_Mask = 0;

		uintptr_t m_ReturnValue = 0;
		for (uintptr_t i = 0; m_Size > i; ++i)
		{
			if (Signature_IsMatch(reinterpret_cast<uint8_t*>(m_Address + i), m_PatternAlloc, m_MaskAlloc))
			{
				m_ReturnValue = (m_Address + i);
				break;
			}
		}

		delete[] m_PatternAlloc;
		delete[] m_MaskAlloc;
		return m_ReturnValue;
	}

	DWORD GetPid(const wchar_t* process_name) {
		DWORD process_id = 0;

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (snapshot == INVALID_HANDLE_VALUE)
			return process_id;

		PROCESSENTRY32W entry = {};
		entry.dwSize = sizeof(entry);

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

		this->driv = handle;

		return DeviceIoControl(handle, DriverCodes::Attach, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
	}

	template <class T>
	T ReadMem(const uintptr_t addr) {
		T temp = {};

		DriverRequest r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = &temp;
		r.size = sizeof(T);

		DeviceIoControl(this->driv, DriverCodes::Read, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);

		return temp;
	}

	bool ReadRawMem(const uintptr_t addr, LPVOID buffer, SIZE_T size) {

		DWORD bytesRead;

		DriverRequest r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = buffer;
		r.size = size;

		if (DeviceIoControl(this->driv, DriverCodes::Read, &r, sizeof(r), &r, sizeof(r), &bytesRead, nullptr))
			return size == bytesRead;

		return FALSE;
	}

	template <class T>
	void WriteMem(uintptr_t addr, const T& value) {
		DriverRequest r;
		r.target = reinterpret_cast<PVOID>(addr);
		r.buffer = (PVOID)&value;
		r.size = sizeof(T);

		DeviceIoControl(this->driv, DriverCodes::Write, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
	}
} Driver;