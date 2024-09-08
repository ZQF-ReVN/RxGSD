#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <ZxHook/SHooker.h>


namespace ZQF::RxGSD
{
	template <class MEM_STRCUT_TYPE>
	struct VFSHook
	{
	private:
		using VER_TYPE = MEM_STRCUT_TYPE;

	private:
		static inline char sg_aHookFolder[256]{ };
		static inline VER_TYPE::VFS::Fn_LoadFileViaDir_t sg_fnLoadFileViaDir{};

	private:
		static auto __fastcall LoadFileViaPack_Hook(VER_TYPE::VFS* This, std::uint32_t EDX, VER_TYPE::STD_String msPackName, VER_TYPE::STD_String msFileName, std::uint8_t ucZLC3Flag) -> bool
		{
			char full_path[512];
			std::memcpy(full_path, sg_aHookFolder, sizeof(sg_aHookFolder) - 1);
			std::memcpy(full_path + sizeof(sg_aHookFolder) - 1, msFileName.C_Str(), msFileName.Size());
			full_path[sizeof(sg_aHookFolder) - 1 + msFileName.Size()] = {};

			if ((::GetFileAttributesA(full_path) != INVALID_FILE_ATTRIBUTES) && msFileName.Size())
			{
				msFileName.Assign(full_path);
				return sg_fnLoadFileViaDir(This, msFileName, ucZLC3Flag);
			}

			return ZxHook::SHooker<LoadFileViaPack_Hook>::FnRaw(This, EDX, msPackName, msFileName, ucZLC3Flag);
		}

	public:
		static auto Install(std::string_view msHookDir, std::size_t vaFnLoadFileViaPack, std::size_t vaFnLoadFileViaDir, std::size_t vaFnStdStrAssign) -> void
		{
			std::memcpy(sg_aHookFolder, msHookDir.data(), msHookDir.size());
			sg_aHookFolder[msHookDir.size()] = {};

			sg_fnLoadFileViaDir = reinterpret_cast<VER_TYPE::VFS::Fn_LoadFileViaDir_t>(vaFnLoadFileViaDir);
			VER_TYPE::STD_String::FnAssign = reinterpret_cast<VER_TYPE::STD_String::Fn_Assign_t>(vaFnStdStrAssign);
			ZxHook::SHooker<LoadFileViaPack_Hook>::Commit(vaFnLoadFileViaPack);
		}
	};
}
