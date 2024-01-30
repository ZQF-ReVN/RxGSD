#pragma once
#include <string>
#include <vector>
#include <filesystem>

#include "GSD_Types.h"


namespace GSD
{
	class GSP
	{
	public:
		static void Extract(const std::filesystem::path& phPack, const std::filesystem::path& phFolder);
		static void Pack(const std::filesystem::path& phFolder, const std::filesystem::path& phPack);
	};
}