#pragma once
#include <d3d9.h>
#include "utils.h";

namespace Goo {
	inline bool open = true;

	inline bool setup = false;

	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC ogWindowProcess = nullptr;

	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool setupWindowClass(const char* windowClassName) noexcept;
	void destroyWindowClass() noexcept;

	bool setupWindow(const char* windowName) noexcept;
	bool destroyWindow() noexcept;

	bool setupDirectX() noexcept;
	bool destroyDirectX() noexcept;

	void Setup();

	void setupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void destroy();

	void render();
}