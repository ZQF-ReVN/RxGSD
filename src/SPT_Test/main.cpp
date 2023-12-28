#include <iostream>
#include <format>
#include <filesystem>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxFile.h"
#include "../../lib/Rut/RxMem.h"
#include "../../lib/Rut/RxJson.h"
#include "../../lib/Rut/RxBench.h"


int main()
{
	try
	{
		std::filesystem::create_directory(L"spt_json/");
		std::filesystem::create_directory(L"spt_dump/");

		for (auto& entry : std::filesystem::directory_iterator(L"spt/"))
		{
			if (entry.is_regular_file() == false) { continue; }
			if (entry.path().extension() != L".spt") { continue; }

			const std::filesystem::path& sdt_path = entry.path();
			std::wcout << L"Parser: " << sdt_path.wstring() << L"  ";
			{
				Rut::RxMem::Auto spt{ L"spt_dec/" + sdt_path.wstring() };
				GSD::SPT::Parser parser;
				parser.Load(spt.GetPtr());
				parser.Make().SaveData(L"spt_dump/" + sdt_path.wstring());
				GSD::SPT::CheckDumpDataMatched(spt.GetPtr(), parser);
				auto json = parser.Make(932);
				Rut::RxJson::Parser::Save(json, L"spt_json/" + sdt_path.wstring() + L".json", true, true);
			}
			std::wcout << L"OK" << L'\n';
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}