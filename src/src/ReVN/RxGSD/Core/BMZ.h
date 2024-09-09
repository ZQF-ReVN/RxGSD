#pragma once
#include <string_view>


namespace ZQF::ReVN::RxGSD
{
	class BMZ
	{
	public:
		static auto Export(const std::string_view msBMZPath, const std::string_view msBMPPath) -> void;
		static auto Import(const std::string_view msBMZPath, const std::string_view msBMPPath, const std::string_view msSavePath) -> void;
	};
}