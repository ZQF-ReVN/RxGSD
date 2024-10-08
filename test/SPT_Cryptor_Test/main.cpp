﻿#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <Zut/ZxFS.h>
#include <Zut/ZxMem.h>
#include <ReVN/RxGSD/Core/SPT_Cryptor.h>

namespace RxGSD { using namespace ZQF::ReVN::RxGSD; }


auto main(void) -> int
{
	try
	{
		std::string_view save_dir{ "spt_dec/" };
		for (ZxFS::Walker walker{ "data/" }; walker.NextFile();)
		{
			ZxMem spt_file{ walker.GetPath() };
			RxGSD::SPT::Cryptor::Decode(spt_file.Span(), true);
			spt_file.Save(std::string{ save_dir }.append(walker.GetName()));
			std::println("decrypt -> {}", walker.GetPath());
		}
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}