#include <print>
#include <iostream>
#include <RxGSD/Core/GSP.h>


auto main(void) -> int
{
	try
	{
		ZQF::RxGSD::GSP::Export("data.gsp", "data/");
		//ZQF::RxGSD::GSP::Import("data/", "image3.new.gsp");
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}