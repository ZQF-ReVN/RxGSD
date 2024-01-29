#include "SPT_Str.h"
#include "Rut/RxStr.h"
#include "Rut/RxCmd.h"

#include <stdexcept>


namespace GSD::SPT
{
	std::wstring Str::NumToStr(const wchar_t* wpFormat, size_t nValue)
	{
		wchar_t buf[0x10];
		size_t len = (size_t)swprintf_s(buf, 0x10, wpFormat, nValue);
		return { buf, len };
	}

	size_t Str::StrToNum(const wchar_t* wpFormat, std::wstring_view wsStr)
	{
		size_t value = 0;
		size_t len = (size_t)swscanf_s(wsStr.data(), wpFormat, &value);
		return value;
	}

	std::wstring Str::LoadANSI(const std::string_view msStr, size_t uCodePage)
	{
		return Rut::RxStr::ToWCS(msStr, uCodePage);
	}

	std::string Str::MakeANSI(const std::wstring_view wsStr, size_t uCodePage)
	{
		return Rut::RxStr::ToMBCS(wsStr, uCodePage);
	}

	std::vector<SPT_Char_Entry> Str::MakeCharTable(const std::string& msText)
	{
		std::vector<SPT_Char_Entry> char_entry_list;
		{
			size_t notation_pos_beg = 0;
			SPT_Char_Entry entry = { 0 };
			for (size_t ite_char = 0; ite_char < msText.size(); ite_char++)
			{
				if ((uint8_t)msText[ite_char] >= 0x81)
				{
					uint32_t dbcs_char_l = (msText[ite_char + 0]) & 0xFF;
					uint32_t dbcs_char_h = (msText[ite_char + 1]) & 0xFF;
					ite_char++;
					entry.uiType = 0x7;
					entry.uiParam0 = 0x0;
					entry.uiParan1 = (dbcs_char_l) | (dbcs_char_h << 0x8);
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
						if (notation_pos_beg == -1)
						{
							throw std::runtime_error("Notation token mismatched");
						}

						entry.uiType = 0x8; // notation beg
						entry.uiParam0 = (uint32_t)(char_entry_list.size() - notation_pos_beg);
						entry.uiParan1 = 0;

						notation_pos_beg = -1;
					}
					else if (msText[ite_char] == '>')
					{
						entry.uiType = 0x9; // notation end
						entry.uiParam0 = 0;
						entry.uiParan1 = 0;
					}
					else
					{
						entry.uiType = 0x7; // normal char
						entry.uiParam0 = 0x0;
						entry.uiParan1 = msText[ite_char];
					}
				}

				char_entry_list.push_back(entry);
			}

			entry.uiType = 0xD; // end flag
			entry.uiParam0 = 0x0;
			entry.uiParan1 = 0;
			char_entry_list.push_back(entry);
		}
		return char_entry_list;
	}

	std::string Str::ParseCharTable(const std::vector<SPT_Char_Entry>& rfCharEntries)
	{
		std::vector<uint32_t> char_list;
		{
			for (auto& char_entry : rfCharEntries)
			{
				switch (char_entry.uiType)
				{
				case 0x7:// normal char flag
				{
					char_list.push_back(char_entry.uiParan1);
				}
				break;

				case 0x8:// notation beg flag
				{
					char_list.insert(char_list.end() - char_entry.uiParam0, '<');
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
					Rut::RxCmd::PutFormat(L"\n\tParseSptText: Missing control character[Type:0x%08x, Param0:0x%08x, Param1:0x%08x]\n", char_entry.uiType, char_entry.uiParam0, char_entry.uiParan1);
				}
				}
			}
		}

		std::string text;
		for (auto dbcs_char : char_list)
		{
			char dbcs_char_l = (char)((dbcs_char >> 0) & 0x00FF);
			char dbcs_char_h = (char)((dbcs_char >> 8) & 0x00FF);
			((uint8_t)dbcs_char_l >= 0x81u) ? (void)(text.append(1, dbcs_char_l), text.append(1, dbcs_char_h)) : (void)(text.append(1, dbcs_char_l));
		}

		return text;
	}
}