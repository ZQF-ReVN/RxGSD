#include <iostream>

#include "lib/GSD/BMZ.h"
#include "lib/Rut/RxPath.h"
#include "lib/Rut/RxMem.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 2:
		{
			std::wstring path = argv[1];
			std::wstring stuffix = Rut::RxPath::GetSuffix(path);
			if (stuffix == L".bmz")
			{
				GSD::BMZ::Cryptor::Decode(path, Rut::RxPath::RemoveSuffix(path) + L".bmp");
			}
		}
		break;

		case 4:
		{
			std::wstring_view command = argv[1];

			if (command == L"decode")
			{
				std::wstring_view bmz_path = argv[2];
				std::wstring_view bmp_path = argv[3];
				GSD::BMZ::Cryptor::Decode(bmz_path, bmp_path);
				std::cout << "Success\n";
			}
		}
		break;

		case 5:
		{
			std::wstring_view command = argv[1];
			if (command == L"encode")
			{
				std::wstring_view org_bmz_path = argv[2];
				std::wstring_view bmp_path = argv[3];
				std::wstring_view new_bmz_path = argv[4];
				GSD::BMZ::Cryptor::Encode(org_bmz_path, bmp_path, new_bmz_path);
				std::cout << "Success\n";
			}
		}
		break;

		default:
		{
			std::cout
				<< "Command:\n"
				<< "\tBMZ_Editor.exe decode [bmz_path] [bmp_path]\n"
				<< "\tBMZ_Editor.exe encode [org_bmz_path] [bmp_path] [new_bmz_path]\n"
				<< "Example:\n"
				<< "\tBMZ_Editor.exe decode cg101_01.bmz cg101_01.bmz.bmp\n"
				<< "\tBMZ_Editor.exe encode cg101_01.bmz cg101_01.bmz.bmp cg101_01.bmz.new\n\n";
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
	::UserMain(argc, argv);
}