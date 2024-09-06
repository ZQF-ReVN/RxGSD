#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <ZxFS/Core.h>
#include <ZxFS/Walker.h>
#include <ZxMem/ZxMem.h>
#include <RxGSD/Core/SPT_HDR.h>


auto main(void) -> int
{
	try
	{
		for (ZQF::ZxFS::Walker walker{"spt_dec/"}; walker.NextFile();)
		{
			if (walker.IsSuffix(".spt") == false) { continue; }

			ZQF::ZxMem spt_org_mem{ walker.GetPath() };

			ZQF::RxGSD::SPT::HDR spt_hdr_org;
			spt_hdr_org.Load(spt_org_mem); // load via mem
			ZQF::ZxMem spt_hdr_org_dump_mem{ spt_hdr_org.Make() };
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

			ZQF::ZxJson::JValue spt_hdr_json{ spt_hdr_org.Make(932) }; // dump json
			ZQF::RxGSD::SPT::HDR spt_hdr_reload;
			spt_hdr_reload.Load(spt_hdr_json, 932); // load via json
			ZQF::ZxMem spt_hdr_reload_dump_mem{ spt_hdr_reload.Make() };
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