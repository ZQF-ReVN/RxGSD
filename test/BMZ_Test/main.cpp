#include <print>
#include <iostream>
#include <ReVN/RxGSD/Core/BMZ.h>

namespace RxGSD { using namespace ZQF::ReVN::RxGSD; }


auto main(void) -> int
{
	try
	{
		// ZQF::RxGSD::BMZ::Export("Sys_config_win_full.bmz", "Sys_config_win_full.bmp");
		RxGSD::BMZ::Import("Sys_config_win_full.bmz", "Sys_config_win_full.bmp", "Sys_config_win_full.bmz.new");
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}