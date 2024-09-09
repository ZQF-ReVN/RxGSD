#include <Windows.h>
#include <ReVN/RxGSD/Hook/GSD_MStruct.h>
#include <ReVN/RxGSD/Hook/VFS_Hook.h>

namespace RxGSD { using namespace ZQF::ReVN::RxGSD; }


static auto StartHook(HMODULE /* hDllBase */) -> void
{
    const auto exe_va{ reinterpret_cast<std::size_t>(::GetModuleHandleW(nullptr)) };

	// 他の男の精液で孕んでもいいですか…？３ PKG
    {
        // RxGSD::VFSHook<RxGSD::MStruct::Ver3018>::Install("VFS\\", exe_va + 0x1A30A0, exe_va + 0x1A31D0, exe_va + 0x8140);
    }

    // ボクの目の前で親友に抱かれ腰を振る彼女 ―奴の言いなりにそのエロい肢体は開かれていく― PKG
    {
        RxGSD::VFSHook<RxGSD::MStruct::Ver3029>::Install("VFS\\", exe_va + 0x19AB10, exe_va + 0x19AC40, exe_va + 0x8110);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID /* lpReserved */)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ::StartHook(hModule);
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