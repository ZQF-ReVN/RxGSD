#include <print>
#include <iostream>
#include <ReVN/RxGSD/Core/GSP.h>

namespace RxGSD { using namespace ZQF::ReVN::RxGSD; }


auto main(void) -> int
{
	try
	{
		RxGSD::GSP::Export("data.gsp", "data/");
		//RxGSD::GSP::Import("data/", "image3.new.gsp");
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}