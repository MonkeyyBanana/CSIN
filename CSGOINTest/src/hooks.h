#pragma once
#include "gui.h"

namespace Hooks {
	void setup();
	void destroy() noexcept;

	constexpr void* virtFunction(void* thisptr, int index) noexcept {
		return (*static_cast<void***>(thisptr))[index];
	}

	using endScenceFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline endScenceFn endSceneOriginal = nullptr;
	long __stdcall endScene(IDirect3DDevice9* device) noexcept;

	using resetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline resetFn resetOriginal = nullptr;
	HRESULT __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;
}

