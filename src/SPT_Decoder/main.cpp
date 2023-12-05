#include <iostream>
#include <vector>

#include "../../lib/GSD/SPT.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxMem.h"


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 2:
		{
			std::wstring spt_folder = argv[1];
			if (spt_folder.back() != L'/') { spt_folder.append(1, L'/'); }

			std::vector<std::wstring> file_list;
			Rut::RxPath::CurFileNames(spt_folder, file_list, false);
			for (auto& file_name : file_list)
			{
				Rut::RxMem::Auto spt_file{ spt_folder + file_name };
				GSD::SPT::Coder::Decode(spt_file.GetPtr(), spt_file.GetSize(), true);
				spt_file.SaveData(L"spt_dec/" + file_name);
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