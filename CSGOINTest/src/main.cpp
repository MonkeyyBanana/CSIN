#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <thread>

void setup(const HMODULE process) {

}

int __stdcall DllMain(const HMODULE process, const std::uintptr_t reason, const void* reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(process);

		const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(setup), process, 0, nullptr);

		if (thread)
			CloseHandle(thread);
	}

	return TRUE;
}


















/*
namespace displace {
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA964;
	constexpr ::std::ptrdiff_t dwForceJump = 0x52BBC7C;

	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
}

void bunnyHop(const HMODULE instance) noexcept {
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

	while (!GetAsyncKeyState(VK_END)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!GetAsyncKeyState(VK_SPACE))
			continue;

		const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(client + displace::dwLocalPlayer);

		if (!localPlayer)
			continue;

		const auto health = *reinterpret_cast<std::int32_t*>(localPlayer + displace::m_iHealth);

		if (!health)
			continue;

		const auto flags = *reinterpret_cast<std::int32_t*>(localPlayer + displace::m_fFlags);

		(flags & (1 << 0)) ? *reinterpret_cast<std::uintptr_t*>(client + displace::dwForceJump) = 6 : *reinterpret_cast<std::uintptr_t*>(client + displace::dwForceJump) = 4;
	}

	FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(const HMODULE instance, const std::uintptr_t reason, const void* reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(bunnyHop), instance, 0, nullptr);

		if (thread)
			CloseHandle(thread);
	}

	return TRUE;
}
*/