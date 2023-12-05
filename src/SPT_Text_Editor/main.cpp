#include <iostream>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxConsole.h"


bool ExportText(const std::wstring_view wsScriptPath, const std::wstring_view wsJsonPath, size_t nCodePage)
{
	// Parse SPT Script
	GSD::SPT::Parser spt_parser;
	spt_parser.Parse(wsScriptPath);

	// Find Code With Text 
	std::vector<std::vector<GSD::SPT::SPT_Code>::iterator> code_with_text_list;
	{
		std::vector<GSD::SPT::SPT_Code>& code_list = spt_parser.GetCodeList();

		for (auto ite = code_list.begin(); ite != code_list.end(); ite++)
		{
			const GSD::SPT::SPT_Code_Parameter_Type0& type0 = ite->GetParamType0();
			if (type0.GetType0TextLen())
			{
				code_with_text_list.push_back(ite);
			}
		}

		if (code_with_text_list.empty())
		{
			Rut::RxConsole::PutFormat(L"Not Find Text:%s\n", wsScriptPath.data());
			return false;
		}
	}

	// Append Text To Json
	Rut::RxJson::Value json;
	for (auto& ite : code_with_text_list)
	{
		std::string text = ite->GetParamType0().GetType0Text();
		json.Append(Rut::RxStr::ToWCS(text, nCodePage));
	}

	// Save Json
	Rut::RxJson::Parser::Save(json, wsJsonPath, true);

	return true;
}

bool ImportText(const std::wstring_view wsScriptPath, const std::wstring_view wsJsonPath, const std::wstring_view wsScriptNewPath, size_t nCodePage)
{
	// Parse SPT Script
	GSD::SPT::Parser spt_parser;
	spt_parser.Parse(wsScriptPath);

	// Find Code With Text
	std::vector<std::vector<GSD::SPT::SPT_Code>::iterator> code_with_text_list;
	{
		std::vector<GSD::SPT::SPT_Code>& code_list = spt_parser.GetCodeList();

		for (auto ite = code_list.begin(); ite != code_list.end(); ite++)
		{
			if (ite->GetParamType0().GetType0TextLen())
			{
				code_with_text_list.push_back(ite);
			}
		}

		if (code_with_text_list.empty())
		{
			Rut::RxConsole::PutFormat(L"Not Find Text:%s\n", wsScriptPath.data());
			return false;
		}
	}

	// Parse Text Json
	Rut::RxJson::Value msg_json;
	Rut::RxJson::Parser{}.Load(wsJsonPath, msg_json);
	Rut::RxJson::JArray& msg_list = msg_json.ToAry();

	if (code_with_text_list.size() != msg_list.size())
	{
		Rut::RxConsole::PutFormat(L"Text Count Mismatch :%s\n", wsJsonPath.data());
		return false;
	}

	// Import Text
	for (size_t ite = 0; ite < msg_list.size(); ite++)
	{
		const std::wstring_view text_w = msg_list[ite].ToStringView();
		std::string text = Rut::RxStr::ToMBCS(text_w, nCodePage);
		code_with_text_list[ite]->GetParamType0().SetType0Text(text);
	}

	// Save SPT Script
	spt_parser.Dump().SaveData(wsScriptNewPath);

	return true;
}

int wmain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 5:
		{
			std::wstring_view spt_path = argv[2];
			std::wstring_view json_path = argv[3];
			size_t code_page = _wtoi(argv[4]);
			ExportText(spt_path, json_path, code_page) ? (std::cout << "Success\n") : (std::cout << "Failed\n");
		}
		break;

		case 6:
		{
			std::wstring_view spt_path = argv[2];
			std::wstring_view json_path = argv[3];
			std::wstring_view spt_new_path = argv[4];
			size_t code_page = _wtoi(argv[5]);
			ImportText(spt_path, json_path, spt_new_path, code_page) ? (std::cout << "Success\n") : (std::cout << "Failed\n");
		}
		break;

		default:
		{
			std::cout
				<< "Command:\n"
				<< "\tSPT_Text_Editor.exe e [spt_path] [json_path] [codepage]\n"
				<< "\tSPT_Text_Editor.exe i [spt_path] [json_path] [spt_new_path] [codepage]\n"
				<< "Example:\n"
				<< "\tSPT_Text_Editor.exe e 0scene_pro001.spt 0scene_pro001.spt.json 932\n"
				<< "\tSPT_Text_Editor.exe i 0scene_pro001.spt 0scene_pro001.spt.json 0scene_pro001.spt.new 936\n\n";
		}
		}

	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}