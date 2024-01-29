#include <iostream>
#include <vector>
#include <filesystem>

#include "GSD/SPT_File.h"
#include "GSD/SPT_Cryptor.h"
#include "Rut/RxMem.h"
#include "Rut/RxCmd.h"
#include "Rut/RxPath.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-spt", L"spt file path");
		arg.AddCmd(L"-json", L"json file path");
		arg.AddCmd(L"-folder", L"spt folder path");
		arg.AddCmd(L"-save", L"save path");
		arg.AddCmd(L"-code", L"codepage");
		arg.AddCmd(L"-mode", L"mode [single]:decode file, [batch]: ");
		arg.AddCmd(L"-way", L"way [spt2json]:spt to json, [json2spt]: json to spt: ");
		arg.AddExample(L"-mode single -way spt2json -spt 0scene_pro001.spt -save 0scene_pro001.spt.json -code 932");
		arg.AddExample(L"-mode single -way json2spt -json 0scene_pro001.spt.json -save 0scene_pro001.spt.new -code 932");
		arg.AddExample(L"-mode batch -way spt2json -folder spt/ -save spt_json/ -code 932");
		arg.AddExample(L"-mode batch -way json2spt -folder spt_json/ -save spt_new/ -code 932");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"single")
		{
			if (arg[L"-way"] == L"spt2json")
			{
				GSD::SPT::File spt_file;
				spt_file.Load(arg[L"-spt"]);
				Rut::RxJson::JValue spt_json = spt_file.Make(arg[L"-code"]);
				Rut::RxJson::Parser::Save(spt_json, arg[L"-save"], true);
			}
			else if(arg[L"-way"] == L"json2spt")
			{
				GSD::SPT::File spt_file;
				Rut::RxJson::JValue spt_json = Rut::RxJson::Parser{}.Load(arg[L"-json"]);
				spt_file.Load(spt_json,arg[L"-code"]);
				spt_file.Make().SaveData(arg[L"-save"]);
			}
		}
		else if (arg[L"-mode"] == L"batch")
		{
			if (arg[L"-way"] == L"spt2json")
			{
				size_t code_page = arg[L"-code"];
				std::filesystem::path save_path = arg[L"-save"];
				std::filesystem::create_directories(save_path);
				for (auto& entry : std::filesystem::directory_iterator(arg[L"-folder"]))
				{
					if (entry.is_regular_file() == false) { continue; }
					if (entry.path().extension() != L".spt") { continue; }

					GSD::SPT::File spt_file;
					spt_file.Load(entry.path().wstring());
					Rut::RxJson::JValue spt_json = spt_file.Make(code_page);
					Rut::RxJson::Parser::Save(spt_json, save_path / entry.path().stem().replace_extension(L".json"), true);
				}
			}
			else if (arg[L"-way"] == L"json2spt")
			{
				size_t code_page = arg[L"-code"];
				std::filesystem::path save_path = arg[L"-save"];
				std::filesystem::create_directories(save_path);
				for (auto& entry : std::filesystem::directory_iterator(arg[L"-folder"]))
				{
					if (entry.is_regular_file() == false) { continue; }

					GSD::SPT::File spt_file;
					Rut::RxJson::JValue spt_json = Rut::RxJson::Parser{}.Load(entry);
					spt_file.Load(spt_json, code_page);
					spt_file.Make().SaveData(save_path / entry.path().stem().replace_extension(L".spt"));
				}
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
	std::filesystem::path save_path = L"spt_dec";
	for (auto& entry : std::filesystem::directory_iterator("spt"))
	{
		if (entry.is_regular_file() == false) { continue; }
		const std::filesystem::path& spt_path = entry.path();
		Rut::RxMem::Auto spt_file{ spt_path.wstring() };
		GSD::SPT::Cryptor::Decode(spt_file, true);
		spt_file.SaveData(save_path / spt_path.filename());
	}
}

int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
	//::DebugMain();
}