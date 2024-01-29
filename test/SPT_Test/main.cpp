#include <iostream>
#include <format>
#include <filesystem>

#include "GSD/SPT_File.h"
#include "Rut/RxFile.h"
#include "Rut/RxMem.h"
#include "Rut/RxJson.h"
#include "Rut/RxBench.h"
#include "Rut/RxCmd.h"


static void TestMake()
{
	try
	{
		std::filesystem::create_directory(L"spt_json_meta/");
		std::filesystem::create_directory(L"spt_dump/");

		for (auto& entry : std::filesystem::directory_iterator(L"spt_dec/"))
		{
			if (entry.is_regular_file() == false) { continue; }
			if (entry.path().extension() != L".spt") { continue; }

			const std::filesystem::path& sdt_path = entry.path();
			std::wcout << L"Parser: " << sdt_path.wstring() << L"  ";
			{
				Rut::RxMem::Auto spt{ sdt_path };
				GSD::SPT::File parser;
				parser.Load(spt.GetPtr());
				parser.Make().SaveData(L"spt_dump/" + sdt_path.filename().wstring());
				GSD::SPT::CheckDumpDataMatched(spt.GetPtr(), parser);
				auto json = parser.Make(932);
				Rut::RxJson::Parser::Save(json, L"spt_json_meta/" + sdt_path.filename().wstring() + L".json", true, true);
			}
			std::wcout << L"OK" << L'\n';
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

static void TestHdrParse()
{
	size_t success_count = 0;
	std::filesystem::create_directory(L"spt_hdr_json/");
	for (auto& entry : std::filesystem::directory_iterator(L"spt_dec/"))
	{
		if (entry.is_regular_file() == false) { continue; }
		if (entry.path().extension() != L".spt") { continue; }

		Rut::RxMem::Auto spt_org_mem(entry);

		size_t spt_real_size = 0;

		GSD::SPT::File spt_file1;
		spt_file1.Load(spt_org_mem.GetPtr());
		Rut::RxJson::JValue spt_file1_json = spt_file1.Make(932);

		spt_real_size = spt_file1.GetSize();

		GSD::SPT::File spt_file2;
		spt_file2.Load(spt_file1_json, 932);

		Rut::RxMem::Auto spt_file1_mem = spt_file1.Make();
		Rut::RxMem::Auto spt_file2_mem = spt_file2.Make();

		GSD::SPT::File spt_file3;
		spt_file3.Load(spt_file1_mem.GetPtr());

		// Check Reload Data
		if (spt_file1_mem.GetSize() == spt_file2_mem.GetSize())
		{
			if (memcmp(spt_file1_mem.GetPtr(), spt_file2_mem.GetPtr(), spt_file1_mem.GetSize()) != 0)
			{
				Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
				continue;
			}
		}
		else
		{
			Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
			continue;
		}

		// Check Org Data
		if (spt_file1_mem.GetSize() == spt_real_size)
		{
			if (memcmp(spt_file1_mem.GetPtr(), spt_org_mem.GetPtr(), spt_real_size) != 0)
			{
				Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
				continue;
			}
		}
		else
		{
			Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
			continue;
		}

		success_count += 1;
	}

	Rut::RxCmd::PutFormat(L"SUCCESS:%d\n", success_count);
}

int main()
{
	TestHdrParse();
}