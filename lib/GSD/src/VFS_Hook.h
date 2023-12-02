#pragma once
#include <cstdint>


namespace GSD::VFS
{
	void SetHookType0(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor);
	void SetHookType1(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor);
}