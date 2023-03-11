#include "hooks.h"

#include <stdexcept>
#include <intrin.h>

#include "../dep/minhook/minhook.h"

#include "../dep/imgui/imgui.h"
#include "../dep/imgui/imgui_impl_win32.h"
#include "../dep/imgui/imgui_impl_dx9.h"

void Hooks::setup() {
	if (MH_Initialize())
		throw std::runtime_error("Fail minhook");

	if (MH_CreateHook(virtFunction(Goo::device, 42), &endScene, reinterpret_cast<void**>(&endSceneOriginal)))
		throw std::runtime_error("EndHook fail");

	if (MH_CreateHook(virtFunction(Goo::device, 16), &reset, reinterpret_cast<void**>(&resetOriginal)))
		throw std::runtime_error("EndHook fail");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Hook fail");

	Goo::destroyDirectX();
}

void Hooks::destroy() noexcept {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall Hooks::endScene(IDirect3DDevice9* device) noexcept {
	static const void* returnAddress = _ReturnAddress();

	const long result = endSceneOriginal(device, device);

	if (_ReturnAddress() == returnAddress)
		return result;

	if (!Goo::setup)
		Goo::setupMenu(device);

	if (Goo::open)
		Goo::remder();

	return result;
}

HRESULT __stdcall Hooks::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const HRESULT result = resetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}