#include <iostream>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxFile.h"
#include "../../lib/Rut/RxMem.h"


int main()
{
	//Rut::RxMem::Auto spt{ L"global.dat" };
	//SPT_Parser parser;
	//parser.Init(spt.GetPtr());

	std::vector<std::wstring> file_list;
	Rut::RxPath::CurFileNames(L"data_dec/", file_list, false);
	for (auto& file_name : file_list)
	{
		std::wcout << L"start_parser:" << file_name << L"  ";
		Rut::RxMem::Auto spt{ L"data_dec/" + file_name };
		GSD::SPT::SPT_Parser parser;
		parser.Parse(spt.GetPtr());
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