#pragma once
#include <cstdint>


namespace GSD::VFS::T0
{
	void SetHook(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor);
}

namespace GSD::VFS::T1
{
	void SetHook(size_t fnLoadFileViaPack, size_t fnLoadFileViaDir, size_t fnStrCtor);
}