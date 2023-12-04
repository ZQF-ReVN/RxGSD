#include "VFS_Hook.h"
#include "GSD_Types.h"

#include "../../Rut/RxConsole.h"
#include "../../RxHook/RxHook.h"


namespace GSD::VFS
{
	static char sg_aHookFolder[16] = "VFS\\";
	static Fn_GSD_STD_Str_Ctor_T0 sg_fnSTDStrCtor = nullptr;
	static Fn_FS_LoadFileViaDir sg_fnLoadFileViaDir = nullptr;
	static Fn_FS_LoadFileViaPack_Type0 sg_fnLoadFileViaPack_Type0 = nullptr;
	static Fn_FS_LoadFileViaPack_Type1 sg_fnLoadFileViaPack_Type1 = nullptr;


	static bool __fastcall LoadFileViaPack_Hook_Type0(GSD_FS* This, uint32_t uiEDX, GSD_STD_String msPackName, uint32_t uiStrAp0, GSD_STD_String msFileName, uint32_t uiStrAp1, uint32_t uiZLC3Flag)
	{
		Rut::RxConsole::PutFormat("%s\n", msFileName.GetRawStr());
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
			return sg_fnLoadFileViaPack_Type0(This, msPackName, uiStrAp0, msFileName, uiStrAp1, uiZLC3Flag);
		}
	}

	static bool __fastcall LoadFileViaPack_Hook_Type1(GSD_FS* This, uint32_t uiEDX, uint32_t uiStrAp0, GSD_STD_String msPackName, uint32_t uiStrAp1, GSD_STD_String msFileName, uint32_t uiZLC3Flag)
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
			return sg_fnLoadFileViaPack_Type1(This, uiStrAp0, msPackName, uiStrAp1, msFileName, uiZLC3Flag);
		}
	}



	void SetHookType0(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor)
	{
		sg_fnSTDStrCtor = (Fn_GSD_STD_Str_Ctor_T0)fnStrCtor;
		sg_fnLoadFileViaDir = (Fn_FS_LoadFileViaDir)fnLoadFileViaDir;
		sg_fnLoadFileViaPack_Type0 = (Fn_FS_LoadFileViaPack_Type0)fnLoadFileViaPack;
		Rut::RxHook::DetourAttachFunc(&sg_fnLoadFileViaPack_Type0, LoadFileViaPack_Hook_Type0);
	}

	void SetHookType1(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor)
	{
		sg_fnSTDStrCtor = (Fn_GSD_STD_Str_Ctor_T0)fnStrCtor;
		sg_fnLoadFileViaDir = (Fn_FS_LoadFileViaDir)fnLoadFileViaDir;
		sg_fnLoadFileViaPack_Type1 = (Fn_FS_LoadFileViaPack_Type1)fnLoadFileViaPack;
		Rut::RxHook::DetourAttachFunc(&sg_fnLoadFileViaPack_Type1, LoadFileViaPack_Hook_Type1);
	}
}