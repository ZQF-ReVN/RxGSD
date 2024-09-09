#pragma once
#include <string_view>


namespace ZQF::ReVN::RxGSD
{
	class GSP
	{
	public:
		static auto Export(const std::string_view msPackPath, const std::string_view msSavDir) -> void;
		static auto Import(const std::string_view msFilesDir, const std::string_view msSavPath) -> void;
	};
}