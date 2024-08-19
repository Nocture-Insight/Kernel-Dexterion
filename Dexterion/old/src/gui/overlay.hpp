#pragma once

#define WIN32_LEAN_AND_MEAN 

#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>

#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_dx11.h"
#include "../../lib/imgui/imgui_impl_win32.h"

#include "../util/Memory.hpp"
#include "../util/functions.hpp"
#include "../config/config.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Gui {
public:
	HINSTANCE hInstance;
	WNDCLASSEXW windowClass;
	HWND window;

	DXGI_SWAP_CHAIN_DESC swapChain{};
	D3D_FEATURE_LEVEL featureLevels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};
	ID3D11Device* device{ 0 };
	ID3D11DeviceContext* deviceContext{ 0 };
	IDXGISwapChain* loadedSwapChain{ 0 };
	ID3D11RenderTargetView* renderTargetView{ 0 };
	D3D_FEATURE_LEVEL loadedLevel{};
	ID3D11Texture2D* backBuffer{ 0 };

	static bool menutoggle;
	static bool IsMenuOpen();

	WNDCLASSEXW CreateWindowClass(HINSTANCE hInstance, WNDPROC Wndproc, LPCWSTR windowname);
	HWND CreateOWindow(int horizontalSize, int verticallSize);
	void MakeFrameIntoClientArea();
	void MakeDeviceAndSwapChain();
	void InitWindow(int nShowCmd);
	void RenderLoop();
	void SetupImGuiStyle();
	void DestroyOWindow();

	void RenderMenu();
};

inline namespace Settings {
	inline ImFont* NormalText;
	inline ImFont* TitleText;
	inline ImFont* SubTitleText;
	inline ImFont* HighlightText;
	inline ImFont* EspNameText;
}