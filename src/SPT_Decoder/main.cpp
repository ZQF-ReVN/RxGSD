#include <iostream>

#include <Rut/RxMem.h>
#include <Rut/RxCmd.h>
#include <GSD/SPT_File.h>
#include <GSD/SPT_Cryptor.h>


static void UserMain(int argc, wchar_t* argv[])
{
	Rut::RxCmd::Arg arg;
	arg.AddCmd(L"-spt", L"spt file path");
	arg.AddCmd(L"-folder", L"spt folder path");
	arg.AddCmd(L"-save", L"save path");
	arg.AddCmd(L"-able", L"make the engine read spt without encryption");
	arg.AddCmd(L"-mode", L"mode: [single] [batch] ");
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
			Rut::RxMem::Auto spt_file{ entry };
			GSD::SPT::Cryptor::Decode(spt_file, is_readbale);
			spt_file.SaveData(save_path / entry.path().filename());
		}
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