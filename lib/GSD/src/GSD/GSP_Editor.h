#pragma once
#include <string>
#include <vector>

#include "GSD_Types.h"


namespace GSD
{
	class GSP
	{
	public:
		static void Extract(const std::wstring_view wsPackPath, const std::wstring& wsFolderPath);
		static void Pack(const std::wstring_view wsPackPath, const std::wstring& wsFolderPath);
	};
}