#include <iostream>
#include <vector>
#include <filesystem>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxMem.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 2:
		{
			for (auto& entry : std::filesystem::directory_iterator(argv[1]))
			{
				if (entry.is_regular_file() == false) { continue; }

				const std::filesystem::path& spt_path = entry.path();

				Rut::RxMem::Auto spt_file{ spt_path.wstring() };
				GSD::SPT::Coder::Decode(spt_file.GetPtr(), spt_file.GetSize(), true);
				spt_file.SaveData(L"spt_dec/" + spt_path.filename().wstring());
			}
		}
		break;

		case 4:
		{
			std::wstring_view command = argv[1];

			if (command == L"decode")
			{
				std::wstring_view spt_path = argv[2];
				std::wstring_view spt_new_path = argv[3];
				Rut::RxMem::Auto spt_file{ spt_path };
				GSD::SPT::Coder::Decode(spt_file.GetPtr(), spt_file.GetSize(), true);
				spt_file.SaveData(spt_new_path);
				std::cout << "Success\n";
			}
		}
		break;

		default:
		{
			std::cout
				<< "Command:\n"
				<< "\tSPT_Decoder.exe decode [spt_path] [spt_new_path]\n"
				<< "Example:\n"
				<< "\tSPT_Decoder.exe decode 0scene_pro001.spt 0scene_pro001.spt.dec\n\n";
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

}

int wmain(int argc, wchar_t* argv[])
{
	UserMain(argc, argv);
}