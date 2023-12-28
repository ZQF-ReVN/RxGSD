#pragma once
#include <string>


namespace GSD::BMZ
{
	class Coder
	{
	public:
		static void Decode(const std::wstring_view wsBMZ, const std::wstring_view wsBMP);
		static void Encode(const std::wstring_view wsOrgBMZPath, const std::wstring_view wsBMPPath, const std::wstring_view wsNewBMZPath);
	};
}