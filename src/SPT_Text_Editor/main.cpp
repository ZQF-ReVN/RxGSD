#include <iostream>
#include <filesystem>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxStr.h"
#include "../../lib/Rut/RxConsole.h"


static void FindCodeWithText(GSD::SPT::Parser& rfParser, std::vector<std::vector<GSD::SPT::Code>::iterator>& vcIterator_Ret)
{
	std::vector<GSD::SPT::Code>& code_list = rfParser.GetCodeList();
	for (auto ite = code_list.begin(); ite != code_list.end(); ite++)
	{
		if (ite->GetArgType0().GetType0TextLen()) { vcIterator_Ret.push_back(ite); }
	}
}

static bool ExportText(const std::wstring_view wsScriptPath, const std::wstring_view wsJsonPath, size_t nCodePage)
{
	// Parse SPT Script
	GSD::SPT::Parser spt_parser;
	spt_parser.Load(wsScriptPath);

	// Find Code With Text 
	std::vector<std::vector<GSD::SPT::Code>::iterator> code_with_text_list;
	::FindCodeWithText(spt_parser, code_with_text_list);
	if (code_with_text_list.empty())
	{
		Rut::RxConsole::PutFormat(L"Not Find Text: %s\n", wsScriptPath);
		return false;
	}

	// Append Text To Json
	Rut::RxJson::JValue json;
	for (auto& ite : code_with_text_list)
	{
		json.Append(Rut::RxStr::ToWCS(ite->GetArgType0().GetType0Text(), nCodePage));
	}

	// Save Json
	Rut::RxJson::Parser::Save(json, wsJsonPath, true);

	return true;
}

static bool ImportText(const std::wstring_view wsScriptPath, const std::wstring_view wsJsonPath, const std::wstring_view wsScriptNewPath, size_t nCodePage)
{
	// Parse SPT Script
	GSD::SPT::Parser spt_parser;
	spt_parser.Load(wsScriptPath);

	// Find Code With Text
	std::vector<std::vector<GSD::SPT::Code>::iterator> code_with_text_list;
	::FindCodeWithText(spt_parser, code_with_text_list);
	if (code_with_text_list.empty())
	{
		Rut::RxConsole::PutFormat(L"Not Find Text: %s\n", wsScriptPath);
		return false;
	}

	// Parse Text Json
	Rut::RxJson::JValue msg_json;
	Rut::RxJson::Parser{}.Load(wsJsonPath, msg_json);
	Rut::RxJson::JArray& msg_list = msg_json.ToAry();

	if (code_with_text_list.size() != msg_list.size())
	{
		Rut::RxConsole::PutFormat(L"Text Count Mismatch: %s\n", wsJsonPath);
		return false;
	}

	// Import Text
	for (size_t ite = 0; ite < msg_list.size(); ite++)
	{
		const std::wstring_view text_w = msg_list[ite];
		std::string text = Rut::RxStr::ToMBCS(text_w, nCodePage);
		code_with_text_list[ite]->GetArgType0().SetType0Text(text);
	}

	// Save SPT Script
	spt_parser.Make().SaveData(wsScriptNewPath);

	return true;
}


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 2:
		{
			std::filesystem::create_directory(L"spt_json/");

			for (auto& entry : std::filesystem::directory_iterator(argv[1]))
			{
				if (entry.is_regular_file() == false) { continue; }
				if (entry.path().extension() != L".spt") { continue; }

				const std::filesystem::path& sdt_path = entry.path();
				const std::filesystem::path& json_path = L"spt_json" / entry.path().filename().replace_extension(L".json");
				ExportText(sdt_path.wstring(), json_path.wstring(), 932);
			}

			system("pause");
		}
		break;

		case 5:
		{
			std::wstring_view commnad = argv[1];
			if (commnad != L"e") { throw std::runtime_error("Error Commnad!"); }
			std::wstring_view spt_path = argv[2];
			std::wstring_view json_path = argv[3];
			size_t code_page = _wtoi(argv[4]);
			ExportText(spt_path, json_path, code_page) ? (Rut::RxConsole::Put(L"Success\n")) : (Rut::RxConsole::Put("Failed\n"));
		}
		break;

		case 6:
		{
			std::wstring_view commnad = argv[1];
			if (commnad != L"i") { throw std::runtime_error("Error Commnad!"); }
			std::wstring_view spt_path = argv[2];
			std::wstring_view json_path = argv[3];
			std::wstring_view spt_new_path = argv[4];
			size_t code_page = _wtoi(argv[5]);
			ImportText(spt_path, json_path, spt_new_path, code_page) ? (Rut::RxConsole::Put(L"Success\n")) : (Rut::RxConsole::Put("Failed\n"));
		}
		break;

		default:
		{
			Rut::RxConsole::Put(L"Command:\n");
			Rut::RxConsole::Put(L"\tSPT_Text_Editor.exe e [spt_path] [json_path] [codepage]\n");
			Rut::RxConsole::Put(L"\tSPT_Text_Editor.exe i [spt_path] [json_path] [spt_new_path] [codepage]\n");
			Rut::RxConsole::Put(L"Example:\n");
			Rut::RxConsole::Put(L"\tSPT_Text_Editor.exe e 0scene_pro001.spt 0scene_pro001.spt.json 932\n");
			Rut::RxConsole::Put(L"\tSPT_Text_Editor.exe i 0scene_pro001.spt 0scene_pro001.spt.json 0scene_pro001.spt.new 936\n\n");
		}
		}

	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

static void DebugMain()
{
	for (auto& entry : std::filesystem::directory_iterator(L"spt_dec/"))
	{
		if (entry.is_regular_file() == false) { continue; }
		if (entry.path().extension() != L".spt") { continue; }
		const std::filesystem::path& sdt_path = entry.path();
		const std::filesystem::path& sdt_new_path = L"spt_new" / entry.path().filename();
		const std::filesystem::path& json_path = L"spt_json" / entry.path().filename().replace_extension(L".json");
		ImportText(sdt_path.wstring(), json_path.wstring(), sdt_new_path.wstring(), 932);
	}
}


int wmain(int argc, wchar_t* argv[])
{
	UserMain(argc, argv);
}