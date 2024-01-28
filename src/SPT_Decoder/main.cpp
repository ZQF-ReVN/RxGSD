#include <iostream>
#include <vector>
#include <filesystem>

#include "lib/GSD/SPT.h"
#include "RxMem.h"
#include "RxCmd.h"
#include "RxPath.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-spt", L"spt file path");
		arg.AddCmd(L"-folder", L"spt folder path");
		arg.AddCmd(L"-save", L"save path");
		arg.AddCmd(L"-able", L"make the engine read spt without encryption");
		arg.AddCmd(L"-mode", L"mode [single]:decode file, [batch]: ");
		arg.AddExample(L"-mode single -able true -spt 0scene_pro001.spt -save 0scene_pro001.spt.dec");
		arg.AddExample(L"-mode batch -able true -folder spt/ -save spt_dec/");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"single")
		{
			Rut::RxMem::Auto spt_file{ arg[L"-spt"].ToWStrView() };
			GSD::SPT::Cryptor::Decode(spt_file, arg[L"-able"].ToBool());
			spt_file.SaveData(arg[L"-save"]);
		}
		else if (arg[L"-mode"] == L"batch")
		{
			bool is_readbale = arg[L"-able"].ToBool();
			std::filesystem::path save_path = arg[L"-save"];
			for (auto& entry : std::filesystem::directory_iterator(arg[L"-folder"]))
			{
				if (entry.is_regular_file() == false) { continue; }
				const std::filesystem::path& spt_path = entry.path();
				Rut::RxMem::Auto spt_file{ spt_path.wstring() };
				GSD::SPT::Cryptor::Decode(spt_file, is_readbale);
				spt_file.SaveData(save_path / spt_path.filename());
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