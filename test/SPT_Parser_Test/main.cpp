#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <Zut/ZxFS.h>
#include <Zut/ZxMem.h>
#include <RxGSD/Core/SPT_File.h>


auto main(void) -> int
{
	try
	{
		for (ZxFS::Walker walker{"spt_dec/"}; walker.NextFile();)
		{
			if (walker.IsSuffix(".spt") == false) { continue; }

			ZxMem spt_org_mem{ walker.GetPath() };

			ZQF::RxGSD::SPT::File spt_file;
			spt_file.Load(spt_org_mem);
			auto spt_dump_mem{ spt_file.Make() };
			// check org dump data
			if (spt_org_mem.SizeBytes() > spt_dump_mem.SizeBytes())
			{
				if (std::memcmp(spt_org_mem.Ptr(), spt_dump_mem.Ptr(), spt_dump_mem.SizeBytes()) != 0)
				{
					std::println("failed -> {}", walker.GetPath());
					continue;
				}
			}
			else
			{
				std::println("failed -> {}", walker.GetPath());
				continue;
			}


			const auto spt_dump_json{ spt_file.Make(932) };
			ZQF::RxGSD::SPT::File spt_reload_via_json;
			spt_reload_via_json.Load(spt_dump_json, 932);
			const auto spt_reload_via_json_dump_mem{ spt_reload_via_json.Make() };
			// check reload via json dump data
			if (spt_org_mem.SizeBytes() > spt_reload_via_json_dump_mem.SizeBytes())
			{
				if (std::memcmp(spt_org_mem.Ptr(), spt_reload_via_json_dump_mem.Ptr(), spt_reload_via_json_dump_mem.SizeBytes()) != 0)
				{
					std::println("failed -> {}", walker.GetPath());
					continue;
				}
			}
			else
			{
				std::println("failed -> {}", walker.GetPath());
				continue;
			}

			ZQF::RxGSD::SPT::File spt_reload_via_mem;
			spt_reload_via_mem.Load(spt_dump_mem);
			const auto spt_reload_via_mem_dump_mem{ spt_reload_via_json.Make() };
			// check reload via mem dump data
			if (spt_org_mem.SizeBytes() > spt_reload_via_mem_dump_mem.SizeBytes())
			{
				if (std::memcmp(spt_org_mem.Ptr(), spt_reload_via_mem_dump_mem.Ptr(), spt_reload_via_mem_dump_mem.SizeBytes()) != 0)
				{
					std::println("failed -> {}", walker.GetPath());
					continue;
				}
			}
			else
			{
				std::println("failed -> {}", walker.GetPath());
				continue;
			}
		}
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}