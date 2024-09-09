#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <Zut/ZxFS.h>
#include <Zut/ZxMem.h>
#include <RxGSD/Core/SPT_HDR.h>


auto main(void) -> int
{
	try
	{
		for (ZxFS::Walker walker{"spt_dec/"}; walker.NextFile();)
		{
			if (walker.IsSuffix(".spt") == false) { continue; }

			ZxMem spt_org_mem{ walker.GetPath() };

			ZQF::RxGSD::SPT::HDR spt_hdr_org;
			spt_hdr_org.Load(spt_org_mem); // load via mem
			ZxMem spt_hdr_org_dump_mem{ spt_hdr_org.Make() };
			// check [org data] == [dump data]
			if (spt_hdr_org_dump_mem.SizeBytes() == spt_org_mem.PosCur())
			{
				if (std::memcmp(spt_hdr_org_dump_mem.Ptr(), spt_org_mem.Ptr(), spt_hdr_org.SizeBytes()) != 0)
				{
					std::println("failed: {}", walker.GetPath());
					continue;
				}
			}
			else
			{
				std::println("failed: {}", walker.GetPath());
				continue;
			}

			ZxJson::JValue spt_hdr_json{ spt_hdr_org.Make(932) }; // dump json
			ZQF::RxGSD::SPT::HDR spt_hdr_reload;
			spt_hdr_reload.Load(spt_hdr_json, 932); // load via json
			ZxMem spt_hdr_reload_dump_mem{ spt_hdr_reload.Make() };
			// check reload data
			if (spt_hdr_org_dump_mem.SizeBytes() == spt_hdr_reload_dump_mem.SizeBytes())
			{
				if (std::memcmp(spt_hdr_org_dump_mem.Ptr(), spt_hdr_reload_dump_mem.Ptr(), spt_hdr_org_dump_mem.SizeBytes()) != 0)
				{
					std::println("failed: {}", walker.GetPath());
					continue;
				}
			}
			else
			{
				std::println("failed: {}", walker.GetPath());
				continue;
			}
		}
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}