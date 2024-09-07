#include "SPT_Str.h"
#include <ZxCvt/ZxCvt.h>
#include <format>
#include <stdexcept>


namespace ZQF::RxGSD::SPT
{
	auto Str::LoadANSI(const std::string_view msStr, const std::size_t nCodePage) -> std::string
	{
		ZxCvt cvt;
		return std::string{ cvt.MBCSToUTF8(msStr, nCodePage) };
	}

	auto Str::MakeANSI(const std::string_view u8Str, const std::size_t nCodePage) -> std::string
	{
		ZxCvt cvt;
		return std::string{ cvt.UTF8ToMBCS(u8Str, nCodePage) };
	}

	auto Str::MakeCharTable(const std::string_view msText) -> std::vector<SPT::Char_Entry>
	{
		std::vector<SPT::Char_Entry> char_entry_list;
		{
			std::size_t notation_pos_beg{};
			SPT::Char_Entry entry{};
			for (std::size_t ite_char{}; ite_char < msText.size(); ite_char++)
			{
				if ((uint8_t)msText[ite_char] >= 0x81)
				{
					std::uint32_t dbcs_char_l{ (msText[ite_char + 0]) & 0xFFu };
					std::uint32_t dbcs_char_h{ (msText[ite_char + 1]) & 0xFFu };
					ite_char++;
					entry.nType = 0x7;
					entry.nParam0 = 0x0;
					entry.nParan1 = (dbcs_char_l) | (dbcs_char_h << 0x8);
				}
				else
				{
					if (msText[ite_char] == '<')
					{
						notation_pos_beg = char_entry_list.size();
						continue;
					}
					else if (msText[ite_char] == '/')
					{
						if (notation_pos_beg == SIZE_MAX)
						{
							throw std::runtime_error("Notation token mismatched");
						}

						entry.nType = 0x8; // notation beg
						entry.nParam0 = (std::uint32_t)(char_entry_list.size() - notation_pos_beg);
						entry.nParan1 = 0;

						notation_pos_beg = SIZE_MAX;
					}
					else if (msText[ite_char] == '>')
					{
						entry.nType = 0x9; // notation end
						entry.nParam0 = 0;
						entry.nParan1 = 0;
					}
					else
					{
						entry.nType = 0x7; // normal char
						entry.nParam0 = 0x0;
						entry.nParan1 = msText[ite_char];
					}
				}

				char_entry_list.push_back(entry);
			}

			entry.nType = 0xD; // end flag
			entry.nParam0 = 0x0;
			entry.nParan1 = 0;
			char_entry_list.push_back(entry);
		}
		return char_entry_list;
	}

	auto Str::ParseCharTable(const std::vector<SPT::Char_Entry>& rfCharEntries) -> std::string
	{
		std::vector<std::uint32_t> char_list;
		{
			for (auto& char_entry : rfCharEntries)
			{
				switch (char_entry.nType)
				{
				case 0x7:// normal char flag
				{
					char_list.push_back(char_entry.nParan1);
				}
				break;

				case 0x8:// notation beg flag
				{
					char_list.insert(char_list.end() - char_entry.nParam0, '<');
					char_list.push_back('/');
				}
				break;

				case 0x9: // notation end flag
				{
					char_list.push_back('>');
				}
				break;

				case 0xD: // string end flag
				{
					// Nothing
				}
				break;

				default:
				{
					throw std::runtime_error("");
					// Rut::RxCmd::PutFormat(L"\n\tParseSptText: Missing control character[Type:0x%08x, Param0:0x%08x, Param1:0x%08x]\n", char_entry.uiType, char_entry.uiParam0, char_entry.uiParan1);
				}
				}
			}
		}

		std::string text;
		for (const auto dbcs_char : char_list)
		{
			char dbcs_char_l = (char)((dbcs_char >> 0) & 0x00FF);
			char dbcs_char_h = (char)((dbcs_char >> 8) & 0x00FF);
			((uint8_t)dbcs_char_l >= 0x81u) ? (void)(text.append(1, dbcs_char_l), text.append(1, dbcs_char_h)) : (void)(text.append(1, dbcs_char_l));
		}

		return text;
	}
}