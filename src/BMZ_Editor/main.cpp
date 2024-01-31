#include <iostream>

#include "Rut/RxCmd.h"
#include "GSD/BMZ_Cryptor.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-bmz", L"bmz file path");
		arg.AddCmd(L"-bmp", L"bmp file path");
		arg.AddCmd(L"-new", L"new bmz file path");
		arg.AddCmd(L"-mode", L"mode: [bmz2bmp] [bmp2bmz]");
		arg.AddExample(L"-mode bmz2bmp -bmz im106.bmz -bmp im106.bmp");
		arg.AddExample(L"-mode bmp2bmz -bmz im106.bmz -bmp im106.bmp -new im106.bmz.new");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"bmz2bmp")
		{
			GSD::BMZ::Cryptor::Decode(arg[L"-bmz"], arg[L"-bmp"]);
		}
		else if (arg[L"-mode"] == L"bmp2bmz")
		{
			GSD::BMZ::Cryptor::Encode(arg[L"-bmz"], arg[L"-bmp"], arg[L"-new"]);
		}
	}
	catch (const std::exception& err)
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