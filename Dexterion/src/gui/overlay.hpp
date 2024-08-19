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
	void EndRenderLoop();
	void SetupImGuiStyle();
	void DestroyOWindow();

	void RenderMenu();
	void MenuBar();

	void EspRender();

	int TabIndex = 0;
};

inline namespace Settings {
	inline float HEIGHT = 720.f;
	inline float WIDTH = HEIGHT / 4 + HEIGHT / 2;

	inline namespace Text {
		inline ImFont* Normal;
		inline ImFont* Title;
		inline ImFont* SubTitle;
		inline ImFont* Highlight;
		inline ImFont* EspName;
	}

	inline namespace Separator {
		inline float Width = WIDTH / 2;
		inline float Height = HEIGHT / 2 + 20;
		inline float Text = 4.f;
	}
}