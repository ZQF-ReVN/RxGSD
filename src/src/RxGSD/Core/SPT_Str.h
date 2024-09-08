#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <RxGSD/Core/GSD_Struct.h>


namespace ZQF::RxGSD::SPT
{
	class Str
	{
	public:
		template <class T>
		static auto NumToStr(const char* cpFormat, const T nValue) -> std::string
		{
			char buf[32];
#ifdef _WIN32
			const auto len{ static_cast<std::size_t>(::sprintf_s(buf, 32, cpFormat, nValue)) };
#elif __linux__
			const auto len{ static_cast<std::size_t>(::sprintf(buf, cpFormat, nValue)) };
#endif // _Win32

			return { buf, len };
		}

		template <class T>
		static auto StrToNum(const char* cpFormat, std::string_view msStr) -> T
		{
			int value{};
#ifdef _WIN32
			const auto len{ static_cast<std::size_t>(::sscanf_s(msStr.data(), cpFormat, &value)) };
#elif __linux__
			const auto len{ static_cast<std::size_t>(::sscanf(msStr.data(), cpFormat, &value)) };
#endif // _Win32
			return static_cast<T>(len > 0 ? value : 0);
		}

		static auto LoadANSI(const std::string_view msStr, const std::size_t nCodePage) -> std::string;
		static auto MakeANSI(const std::string_view u8Str, const std::size_t nCodePage) -> std::string;
		static auto MakeCharTable(const std::string_view msText) -> std::vector<Struct::SPT_Char_Entry>;
		static auto ParseCharTable(const std::vector<Struct::SPT_Char_Entry>& rfCharEntries) -> std::string;
	};
}