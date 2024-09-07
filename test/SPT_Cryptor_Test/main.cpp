#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <ZxFS/Core.h>
#include <ZxFS/Walker.h>
#include <ZxMem/ZxMem.h>
#include <RxGSD/Core/SPT_Cryptor.h>


auto main(void) -> int
{
	try
	{
		std::string_view save_dir{ "spt_dec/" };
		for (ZQF::ZxFS::Walker walker{ "data/" }; walker.NextFile();)
		{
			ZQF::ZxMem spt_file{ walker.GetPath() };
			ZQF::RxGSD::SPT::Cryptor::Decode(spt_file.Span(), true);
			spt_file.Save(std::string{ save_dir }.append(walker.GetName()));
			std::println("decrypt -> {}", walker.GetPath());
		}
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}