#include "VFS_Hook.h"
#include "GSD_Types.h"

#include "../../RxHook/RxHook.h"


namespace GSD::VFS
{
	static char sg_aHookFolder[16] = "VFS\\";
	static Fn_GSD_STD_Str_Ctor_T0 sg_fnSTDStrCtor = nullptr;
	static Fn_FS_LoadFileViaDir sg_fnLoadFileViaDir = nullptr;
	static Fn_FS_LoadFileViaPack sg_fnLoadFileViaPack = nullptr;


	static bool __fastcall LoadFileViaPack_Hook(GSD_FS* This, size_t uiEDX, GSD_STD_String msPackName, size_t uiStrAp0, GSD_STD_String msFileName, size_t uiStrAp1, size_t uiZLC3Flag)
	{
		char full_path[512];
		strcpy_s(full_path, 512, sg_aHookFolder);
		strcat_s(full_path, 512, msFileName.GetRawStr());

		if ((::GetFileAttributesA(full_path) != INVALID_FILE_ATTRIBUTES) && msFileName.uiLen)
		{
			sg_fnSTDStrCtor(&msFileName, full_path);
			return sg_fnLoadFileViaDir(This, msFileName, uiStrAp1, uiZLC3Flag);
		}
		else
		{
			return sg_fnLoadFileViaPack(This, msPackName, uiStrAp0, msFileName, uiStrAp1, uiZLC3Flag);
		}
	}


	void SetHook(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor)
	{
		sg_fnSTDStrCtor = (Fn_GSD_STD_Str_Ctor_T0)fnStrCtor;
		sg_fnLoadFileViaDir = (Fn_FS_LoadFileViaDir)fnLoadFileViaDir;
		sg_fnLoadFileViaPack = (Fn_FS_LoadFileViaPack)fnLoadFileViaPack;
		Rut::RxHook::DetourAttachFunc(&sg_fnLoadFileViaPack, LoadFileViaPack_Hook);
	}
}