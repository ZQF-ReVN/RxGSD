#include <Windows.h>
#include <stdexcept>

#include <RxHook/API.h>
#include <GSD/VFS_Hook.h>


static void StartHook(HMODULE hDllBase)
{
	size_t exe_base = (size_t)::GetModuleHandleW(nullptr);

	// ボクの目の前で親友に抱かれ腰を振る彼女 ―奴の言いなりにそのエロい肢体は開かれていく― PKG
	Rut::RxHook::HookCreateFontIndirectA(0x86, "Simhei");
	//GSD::VFS::T0::SetHook(exe_base + 0x19AB10, exe_base + 0x19AC40, exe_base + 0x8110);

	// 他の男の精液で孕んでもいいですか…？３ PKG
	GSD::VFS::T1::SetHook(exe_base + 0x1A30A0, exe_base + 0x1A31D0, exe_base + 0x8140);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook(hModule);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" VOID __declspec(dllexport) Dir_A() {}