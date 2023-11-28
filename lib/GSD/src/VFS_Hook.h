#pragma once
#include <cstdint>


namespace GSD::VFS
{
	void SetHook(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor);
}