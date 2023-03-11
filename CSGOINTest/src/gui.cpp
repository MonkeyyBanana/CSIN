#include "gui.h"

#include "../dep/imgui/imgui.h"
#include "../dep/imgui/imgui_impl_win32.h"
#include "../dep/imgui/imgui_impl_dx9.h"

#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

bool Goo::setupWindowClass(const char* windowClassName) noexcept {
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_VREDRAW | CS_HREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	if (!RegisterClassEx(&windowClass))
		return FALSE;
	return TRUE;
}

void Goo::destroyWindowClass() noexcept {
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool Goo::setupWindow(const char* windowName) noexcept {
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 50, 50, 0, 0, windowClass.hInstance, 0);

	if (!window)
		return FALSE;
	return TRUE;
}

bool Goo::destroyWindow() noexcept {
	if (window)
		DestroyWindow(window);
}

bool Goo::setupDirectX() noexcept {
	const HMODULE handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return FALSE;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
	CreateFn create = (CreateFn)GetProcAddress(handle, "Direct3DCreate9");  // maybe replace with reinterpret_cast<CreateFn>

	if (!create)
		return FALSE;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return FALSE;
	
	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleQuality = NULL;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, window, D3DCREATE_DISABLE_DRIVER_MANAGEMENT | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &device) < 0)
		return FALSE;
	return TRUE;
}

bool Goo::destroyDirectX() noexcept {
	if (device) {
		device->Release();
		device = NULL;
	}

	if (d3d9) {
		d3d9->Release();
		d3d9 = NULL;
	}
}

void Goo::Setup() {
	srand((unsigned)time(NULL) * 11);
	if (!setupWindowClass(Utils::randomCharStr(7 + rand() % (64 - 7 + 1))))
		throw std::runtime_error("Failed Window Class");

	if (!setupWindow(Utils::randomCharStr(7 + rand() % (64 - 7 + 1))))
		throw std::runtime_error("Failed Window");

	if (!setupDirectX())
		throw std::runtime_error("Failed device");

	destroyWindow();
	destroyWindowClass();
}

void Goo::setupMenu(LPDIRECT3DDEVICE9 device) noexcept {
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	ogWindowProcess = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess)); // maybe replace with reinterpret_cast<WNDPROC>

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void Goo::destroy() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)ogWindowProcess); // reinterpret_cast<LONG_PTR>

	destroyDirectX();
}

void Goo::render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Banana Trading Interface", &open);
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParam) {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		Goo::open = !Goo::open;

	if (Goo::open && ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
		return 1L;

	return CallWindowProc(Goo::ogWindowProcess, window, message, wideParam, longParam);
}