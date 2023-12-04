#include <iostream>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxFile.h"
#include "../../lib/Rut/RxMem.h"


int main()
{
	Rut::RxMem::Auto spt{ L"data_dec/0scene_pro003_h.spt" };
	GSD::SPT::Parser parser;
	parser.Parse(spt.GetPtr());
	bool status = GSD::SPT::CheckDump(spt.GetPtr(), parser);

	std::vector<std::wstring> file_list;
	Rut::RxPath::CurFileNames(L"data_dec/", file_list, false);
	for (auto& file_name : file_list)
	{
		std::wcout << L"start_parser:" << file_name << L"  ";
		Rut::RxMem::Auto spt{ L"data_dec/" + file_name };
		GSD::SPT::Parser parser;
		parser.Parse(spt.GetPtr());
		auto x = parser.Dump();
		x.SaveData(L"data_dump/" + file_name);

		std::wcout << L"OK" << L'\n';
	}

	int a = 0;

	//std::vector<std::wstring> file_list;
	//Rut::RxPath::CurFileNames(L"data_dec/", file_list, false);
	//for (auto& file_name : file_list)
	//{
	//	GSD::SPT::Text_Editor editor{ L"data_dec/" + file_name };
	//	editor.Extract(L"data_json/" + file_name + L".json", 932);
	//}
}