#include "VFS_Hook.h"
#include "GSD_Types.h"

#include "RxCmd.h"
#include "lib/RxHook/RxHook.h"

namespace GSD::VFS
{
	static char sg_aHookFolder[MAX_PATH] = "VFS\\";
}


namespace GSD::VFS::T0
{
	static Fn_GSD_STD_Str_Ctor_T0 sg_fnSTDStrCtor_T0 = nullptr;
	static Fn_FS_LoadFileViaDir_T0 sg_fnLoadFileViaDir_T0 = nullptr;
	static Fn_FS_LoadFileViaPack_T0 sg_fnLoadFileViaPack_T0 = nullptr;


	bool __fastcall LoadFileViaPack_T0_Hook(GSD_FS* This, uint32_t uiEDX, GSD_STD_String_T0 msPackName, GSD_STD_String_T0 msFileName, uint8_t uiZLC3Flag)
	{
		char full_path[MAX_PATH];
		::strcpy_s(full_path, MAX_PATH, sg_aHookFolder);
		::strcat_s(full_path, MAX_PATH, msFileName.GetRawStr());

		if ((::GetFileAttributesA(full_path) != INVALID_FILE_ATTRIBUTES) && msFileName.uiLen)
		{
			sg_fnSTDStrCtor_T0(&msFileName, full_path);
			return sg_fnLoadFileViaDir_T0(This, msFileName, uiZLC3Flag);
		}
		else
		{
			return sg_fnLoadFileViaPack_T0(This, msPackName, msFileName, uiZLC3Flag);
		}
	}

	void SetHook(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor)
	{
		sg_fnSTDStrCtor_T0 = (Fn_GSD_STD_Str_Ctor_T0)fnStrCtor;
		sg_fnLoadFileViaDir_T0 = (Fn_FS_LoadFileViaDir_T0)fnLoadFileViaDir;
		sg_fnLoadFileViaPack_T0 = (Fn_FS_LoadFileViaPack_T0)fnLoadFileViaPack;
		Rut::RxHook::Detours::AttrachDirectly(&sg_fnLoadFileViaPack_T0, LoadFileViaPack_T0_Hook);
	}

}


namespace GSD::VFS::T1
{
	static Fn_GSD_STD_Str_Ctor_T1 sg_fnSTDStrCtor_T1 = nullptr;
	static Fn_FS_LoadFileViaDir_T1 sg_fnLoadFileViaDir_T1 = nullptr;
	static Fn_FS_LoadFileViaPack_T1 sg_fnLoadFileViaPack_T1 = nullptr;


	bool __fastcall LoadFileViaPack_T1_Hook(GSD_FS* This, uint32_t uiEDX, GSD_STD_String_T1 msPackName, GSD_STD_String_T1 msFileName, uint8_t uiZLC3Flag)
	{
		char full_path[MAX_PATH];
		::strcpy_s(full_path, MAX_PATH, sg_aHookFolder);
		::strcat_s(full_path, MAX_PATH, msFileName.GetRawStr());

		if ((::GetFileAttributesA(full_path) != INVALID_FILE_ATTRIBUTES) && msFileName.uiLen)
		{
			T1::sg_fnSTDStrCtor_T1(&msFileName, full_path);
			return sg_fnLoadFileViaDir_T1(This, msFileName, uiZLC3Flag);
		}
		else
		{
			return T1::sg_fnLoadFileViaPack_T1(This, msPackName, msFileName, uiZLC3Flag);
		}
	}

	void SetHook(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor)
	{
		sg_fnSTDStrCtor_T1 = (Fn_GSD_STD_Str_Ctor_T1)fnStrCtor;
		sg_fnLoadFileViaDir_T1 = (Fn_FS_LoadFileViaDir_T1)fnLoadFileViaDir;
		sg_fnLoadFileViaPack_T1 = (Fn_FS_LoadFileViaPack_T1)fnLoadFileViaPack;
		Rut::RxHook::Detours::AttrachDirectly(&sg_fnLoadFileViaPack_T1, LoadFileViaPack_T1_Hook);
	}
}
