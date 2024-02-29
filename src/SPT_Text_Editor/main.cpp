#include <iostream>
#include <filesystem>

#include <Rut/RxStr.h>
#include <Rut/RxCmd.h>
#include <GSD/SPT_File.h>
#include <GSD/SPT_Global.h>


static bool Export(std::vector<std::wstring>& vcName, const std::filesystem::path& phSpt, const std::filesystem::path& phJson, size_t nCodePage)
{
	GSD::SPT::File spt(phSpt);

	// Append Text To Json
	Rut::RxJson::JArray msg_json;
	for (auto& code : spt.GetCodeList())
	{
		if (code.GetArgType0().GetType0TextLen() == 0) { continue; };

		Rut::RxJson::JValue msg_obj;

		size_t char_name_seq = code.GetArgType0().GetNameSeq();
		if (char_name_seq != -1)
		{
			std::wstring_view char_name = vcName[char_name_seq];
			msg_obj[L"chr_raw"] = char_name;
			msg_obj[L"chr_tra"] = char_name;
		}

		std::wstring msg_text = code.GetArgType0().GetType0Text(nCodePage);
		msg_obj[L"msg_raw"] = msg_text;
		msg_obj[L"msg_tra"] = std::move(msg_text);

		msg_json.emplace_back(std::move(msg_obj));
	}

	// Check Msg
	if (msg_json.empty())
	{
		Rut::RxCmd::PutFormat(L"Not Find Text: %s\n", phSpt.wstring().c_str());
		return false;
	}

	// Save Json
	Rut::RxJson::Parser::Save(msg_json, phJson, true);

	return true;
}

static bool Import(std::vector<std::wstring>& vcName, const std::filesystem::path& phSpt, const std::filesystem::path& phJson, const std::filesystem::path& phSptNew, size_t nCodePage)
{
	GSD::SPT::File spt(phSpt);

	// Parse Text Json
	Rut::RxJson::JValue msg_json_doc = Rut::RxJson::Parser{}.Load(phJson);
	Rut::RxJson::JArray& msg_list = msg_json_doc.ToAry();
	Rut::RxJson::JArray::iterator ite_msg_obj = msg_list.begin();

	// Import Text
	for (auto& code : spt.GetCodeList())
	{
		if (code.GetArgType0().GetType0TextLen() == 0) { continue; };

		std::wstring_view text_wide = (*ite_msg_obj)[L"msg_tra"];
		std::string text_ansi = GSD::SPT::Str::MakeANSI(text_wide, nCodePage);
		code.GetArgType0().SetType0Text(text_ansi);
		ite_msg_obj++;
	}

	if (ite_msg_obj == msg_list.begin())
	{
		Rut::RxCmd::PutFormat(L"Not Find Text: %s\n", phSpt.wstring().c_str());
		return false;
	}

	if (ite_msg_obj != msg_list.end())
	{
		Rut::RxCmd::PutFormat(L"Text Count Mismatch: %s\n", phJson.wstring().c_str());
		return false;
	}

	// Save SPT Script
	spt.Make().SaveData(phSptNew);

	return true;
}


static void UserMain(int argc, wchar_t* argv[])
{
	Rut::RxCmd::Arg arg;
	arg.AddCmd(L"-spt", L"spt file path");
	arg.AddCmd(L"-new", L"new spt file path");
	arg.AddCmd(L"-txt", L"json file path");
	arg.AddCmd(L"-global", L"global.dat file path");
	arg.AddCmd(L"-code", L"codepage");
	arg.AddCmd(L"-mode", L"mode [single_export] [single_import] [batch_export] [batch_import]");
	arg.AddExample(L"-mode single_export -global global.dat -spt 0scene_pro001.spt -txt 0scene_pro001.json -code 932");
	arg.AddExample(L"-mode single_import -global global.dat -spt 0scene_pro001.spt -txt 0scene_pro001.json -new 0scene_pro001.spt.new -code 932");
	arg.AddExample(L"-mode batch_export -global global.dat -spt spt/ -txt spt_txt/ -code 932");
	arg.AddExample(L"-mode batch_import -global global.dat -spt spt/ -txt spt_txt/ -new spt_new/ -code 932");
	if (arg.Load(argc, argv) == false) { return; }

	std::wstring_view mode = arg[L"-mode"].ToWStrView();
	size_t code_page = arg[L"-code"];
	std::filesystem::path spt_path = arg[L"-spt"];
	std::filesystem::path txt_path = arg[L"-txt"];

	GSD::SPT::Global global;
	global.Load(arg[L"-global"]);
	std::vector<std::wstring> name_list = global.GetStrTable(code_page);

	if (mode == L"single_export")
	{
		::Export(name_list, spt_path, txt_path, code_page);
	}
	else if (mode == L"single_import")
	{
		::Import(name_list, spt_path, txt_path, arg[L"-new"], code_page);
	}
	else if (mode == L"batch_export")
	{
		std::filesystem::create_directories(txt_path);
		for (auto& spt_entry : std::filesystem::directory_iterator(spt_path))
		{
			if (spt_entry.is_regular_file() == false) { continue; }
			if (spt_entry.path().extension() != L".spt") { continue; }
			std::filesystem::path txt_name = spt_entry.path().stem().replace_extension(L".json");
			::Export(name_list, spt_entry, txt_path / txt_name, code_page);
		}
	}
	else if (mode == L"batch_import")
	{
		std::filesystem::path spt_new_folder = arg[L"-new"];
		std::filesystem::create_directories(spt_new_folder);
		for (auto& txt_entry : std::filesystem::directory_iterator(txt_path))
		{
			if (txt_entry.is_regular_file() == false) { continue; }
			if (txt_entry.path().extension() != L".json") { continue; }
			std::filesystem::path spt_name = txt_entry.path().stem().replace_extension(L".spt");
			::Import(name_list, spt_path / spt_name, txt_entry, spt_new_folder / spt_name, code_page);
		}
	}
	else
	{
		throw std::runtime_error("Command Error!");
	}

}

int wmain(int argc, wchar_t* argv[])
{
	try
	{
		::UserMain(argc, argv);
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}