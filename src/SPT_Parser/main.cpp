#include <iostream>

#include <Rut/RxMem.h>
#include <Rut/RxCmd.h>
#include <GSD/SPT_File.h>
#include <GSD/SPT_Cryptor.h>


static void SptToJson(const std::filesystem::path& phSpt, const std::filesystem::path& phJson, size_t nCodePage)
{
	GSD::SPT::File spt_file(phSpt);
	Rut::RxJson::JValue spt_json = spt_file.Make(nCodePage);
	Rut::RxJson::Parser::Save(spt_json, phJson, true);
}

static void JsonToSpt(const std::filesystem::path& phJson, const std::filesystem::path& phSpt, size_t nCodePage)
{
	Rut::RxJson::JValue spt_json = Rut::RxJson::Parser{}.Load(phJson);
	GSD::SPT::File spt_file(spt_json, nCodePage);
	spt_file.Make().SaveData(phSpt);
}

static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-spt", L"spt file path");
		arg.AddCmd(L"-json", L"json file path");
		arg.AddCmd(L"-folder", L"spt folder path");
		arg.AddCmd(L"-code", L"codepage");
		arg.AddCmd(L"-mode", L"mode [single]:decode file, [batch]: ");
		arg.AddCmd(L"-way", L"way [spt2json]:spt to json, [json2spt]: json to spt: ");
		arg.AddExample(L"-mode single -way spt2json -spt 0scene_pro001.spt -json 0scene_pro001.spt.json -code 932");
		arg.AddExample(L"-mode single -way json2spt -json 0scene_pro001.spt.json -spt 0scene_pro001.spt.new -code 932");
		arg.AddExample(L"-mode batch -way spt2json -folder spt/ -json spt_json/ -code 932");
		arg.AddExample(L"-mode batch -way json2spt -folder spt_json/ -spt spt_new/ -code 932");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"single")
		{
			if (arg[L"-way"] == L"spt2json")
			{
				::SptToJson(arg[L"-spt"], arg[L"-json"], arg[L"-code"]);
			}
			else if(arg[L"-way"] == L"json2spt")
			{
				::JsonToSpt(arg[L"-json"], arg[L"-spt"], arg[L"-code"]);
			}
		}
		else if (arg[L"-mode"] == L"batch")
		{
			if (arg[L"-way"] == L"spt2json")
			{
				size_t code_page = arg[L"-code"];
				std::filesystem::path save_path = arg[L"-json"];
				std::filesystem::create_directories(save_path);
				for (auto& entry : std::filesystem::directory_iterator(arg[L"-folder"]))
				{
					if (entry.is_regular_file() == false) { continue; }
					if (entry.path().extension() != L".spt") { continue; }
					::SptToJson(entry, save_path / entry.path().stem().replace_extension(L".json"), code_page);
				}
			}
			else if (arg[L"-way"] == L"json2spt")
			{
				size_t code_page = arg[L"-code"];
				std::filesystem::path save_path = arg[L"-spt"];
				std::filesystem::create_directories(save_path);
				for (auto& entry : std::filesystem::directory_iterator(arg[L"-folder"]))
				{
					if (entry.is_regular_file() == false) { continue; }
					::JsonToSpt(entry, save_path / entry.path().stem().replace_extension(L".spt"), code_page);
				}
			}
		}
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
	//::DebugMain();
}