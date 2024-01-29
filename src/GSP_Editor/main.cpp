#include <iostream>

#include "Rut/RxPath.h"
#include "GSD/GSP_Editor.h"


static std::wstring FormatFolderPath(std::wstring wsPath)
{
	Rut::RxPath::Format(wsPath.data(), L'/');
	if (wsPath.back() != '/') { wsPath.append(1, '/'); }
	return wsPath;
}

static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 4:
		{
			std::wstring_view command = argv[1];

			if (command == L"extract")
			{
				std::wstring_view gsp_path = argv[2];
				std::wstring file_folder = ::FormatFolderPath(argv[3]);
				GSD::GSP::Extract(gsp_path, file_folder);
				std::cout << "Success\n";
			}
			else if (command == L"pack")
			{
				std::wstring_view new_gsp_path = argv[3];
				std::wstring file_folder = ::FormatFolderPath(argv[2]);
				GSD::GSP::Pack(new_gsp_path, file_folder);
				std::cout << "Success\n";
			}
		}
		break;

		default:
		{
			std::cout
				<< "Command:\n"
				<< "\tGSP_Editor.exe extract [gsp_path] [file_out_folder]\n"
				<< "\tGSP_Editor.exe pack [file_folder] [gsp_out_path]\n"
				<< "Example:\n"
				<< "\tGSP_Editor.exe extract image.gsp image/\n"
				<< "\tGSP_Editor.exe pack image/ image.gsp\n\n";
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