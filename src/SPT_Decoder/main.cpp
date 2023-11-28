#include "../../lib/GSD/SPT.h"

#include "../../lib/Rut/RxJson.h"
#include "../../lib/Rut/RxFile.h"
#include "../../lib/Rut/RxStr.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxMem.h"

#include <vector>


void Decode()
{
	std::vector<std::wstring> file_list;
	Rut::RxPath::CurFileNames(L"data/", file_list, false);
	for (auto& file_name : file_list)
	{
		Rut::RxMem::Auto spt_file{ L"data/" + file_name };
		GSD::SPT::Decode(spt_file.GetPtr(), spt_file.GetSize());
		spt_file.SaveData(L"data_dec/" + file_name);
	}
}

std::wstring NumToHexStr(const size_t nValue)
{
	wchar_t foa_str[64];
	swprintf_s(foa_str, 64, L"0x%08X", nValue);
	return foa_str;
}

size_t HexStrToNum(const std::wstring_view& wsText)
{
	size_t num = 0;
	(void)swscanf(wsText.data(), L"0x%08X", &num);
	return num;
}

struct SPT_Char_Entry
{
	uint32_t uiVal0; // 0x7
	uint32_t uiVal1; // 0x0
	uint32_t uiVal2; // char
};

struct SPT_Text_Entry
{
	uint32_t uiCharCount;
	uint32_t aUn0[2];
	// SPT_Char aText[uiCharCount];
};


class SPT_Text_Editor
{
public:
	std::wstring m_wsPath;
	Rut::RxMem::Auto m_amSPT;
	std::vector<SPT_Text_Entry*> m_vcTextEntryPtr;

	SPT_Text_Editor(std::wstring_view wsPath)
	{
		this->m_wsPath = wsPath;
		this->m_amSPT.LoadFile(wsPath);
	}

	void FindEntry()
	{
		uint8_t* spt_ptr = this->m_amSPT.GetPtr();
		size_t spt_size = this->m_amSPT.GetSize();

		uint8_t text_code[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

		for (size_t ite_bytes = 0; ite_bytes < spt_size; ite_bytes++)
		{
			uint8_t* cur_ptr = spt_ptr + ite_bytes;
			if (memcmp(cur_ptr, text_code, sizeof(text_code)) == 0)
			{
				SPT_Text_Entry* spt_text_ptr = (SPT_Text_Entry*)(cur_ptr + sizeof(text_code));
				SPT_Char_Entry* spt_char_last_ptr = (SPT_Char_Entry*)((size_t)spt_text_ptr + sizeof(SPT_Text_Entry) + (spt_text_ptr->uiCharCount - 1) * sizeof(SPT_Char_Entry));
				if ((spt_text_ptr->uiCharCount) &&
					(spt_text_ptr->aUn0[0] == 0) &&
					(spt_text_ptr->aUn0[1] == 0) &&
					(spt_char_last_ptr->uiVal0 == 0xD))
				{
					this->m_vcTextEntryPtr.push_back(spt_text_ptr);
				}
			}
		}
	}

	static std::wstring ReadText(SPT_Text_Entry* pEntry)
	{
		std::string text;

		size_t dbcs_char_count = pEntry->uiCharCount - 1;
		SPT_Char_Entry* char_entry_beg = (SPT_Char_Entry*)(pEntry + 1);

		for (size_t ite_char = 0; ite_char < dbcs_char_count; ite_char++)
		{
			uint32_t dbcs_char = char_entry_beg[ite_char].uiVal2;

			char dbcs_char_l = (char)((dbcs_char >> 0) & 0x00FF);
			char dbcs_char_h = (char)((dbcs_char >> 8) & 0x00FF);

			((uint8_t)dbcs_char_l >= 0x81u) ? (void)(text.append(1, dbcs_char_l), text.append(1, dbcs_char_h)) : (void)(text.append(1, dbcs_char_l));
		}

		return Rut::RxStr::ToWCS(text, 932);
	}

	void Extract()
	{
		this->FindEntry();

		Rut::RxJson::Value json;

		Rut::RxJson::JObject text_obj;
		uint8_t* spt_ptr = this->m_amSPT.GetPtr();

		for (SPT_Text_Entry* entry_ptr : this->m_vcTextEntryPtr)
		{
			size_t beg_foa = (size_t)((uint8_t*)entry_ptr - spt_ptr);
			size_t end_foa = beg_foa + sizeof(SPT_Text_Entry) + entry_ptr->uiCharCount * sizeof(SPT_Char_Entry);

			text_obj[L"beg"] = NumToHexStr(beg_foa);
			text_obj[L"end"] = NumToHexStr(end_foa);
			text_obj[L"msg"] = ReadText(entry_ptr);

			json.Append(text_obj);
		}

		Rut::RxJson::Parser::Save(json, L"text.json");
	}

	size_t DbcsCountChar(std::string_view msText)
	{
		size_t count = 0;
		for (auto ite_char = msText.begin(); ite_char != msText.end(); ite_char++)
		{
			if ((uint8_t)ite_char[0] >= 0x81)
			{
				count++;
				ite_char++;
			}
			else
			{
				count++;
			}
		}
		return count;
	}

	Rut::RxMem::Auto MakeStruct(const std::string& msText)
	{
		size_t char_count = DbcsCountChar(msText);

		Rut::RxMem::Auto bin;
		bin.SetSize(sizeof(SPT_Text_Entry) + (char_count + 1) * sizeof(SPT_Char_Entry));
		
		uint8_t* bin_ptr = bin.GetPtr();

		SPT_Text_Entry* text_entry_ptr = (SPT_Text_Entry*)bin_ptr;
		text_entry_ptr->uiCharCount = char_count + 1;
		text_entry_ptr->aUn0[0] = 0;
		text_entry_ptr->aUn0[1] = 0;

		SPT_Char_Entry* char_entry_beg = (SPT_Char_Entry*)(text_entry_ptr + 1);
		for (auto ite_char = msText.begin(); ite_char != msText.end(); ite_char++)
		{
			uint32_t char_value = 0;

			if ((uint8_t)ite_char[0] >= 0x81)
			{
				uint32_t dbcs_char_l = (ite_char[0]) & 0xFF;
				uint32_t dbcs_char_h = (ite_char[1]) & 0xFF;
				ite_char++;

				char_value = (dbcs_char_l) | (dbcs_char_h << 0x8);
			}
			else
			{
				char_value = ite_char[0];
			}

			char_entry_beg->uiVal0 = 0x7;
			char_entry_beg->uiVal1 = 0;
			char_entry_beg->uiVal2 = char_value;
			char_entry_beg++;
		}

		char_entry_beg->uiVal0 = 0xD;
		char_entry_beg->uiVal1 = 0;
		char_entry_beg->uiVal2 = 0;

		return bin;
	}

	void Insert()
	{
		Rut::RxJson::Value json;
		Rut::RxJson::Parser parser;
		parser.Open(L"text.json");
		parser.Read(json);

		Rut::RxJson::JArray& info_list = json.ToAry();

		Rut::RxFile::Binary old_spt{ this->m_wsPath, Rut::RIO_READ };
		Rut::RxFile::Binary new_spt{ this->m_wsPath + L".new", Rut::RIO_WRITE };

		size_t old_spt_size = old_spt.GetSize();

		size_t swap_beg = 0;
		size_t swap_end = 0;
		Rut::RxMem::Auto swap_buffer;
		for (auto& info : info_list)
		{
			size_t beg = HexStrToNum(info[L"beg"]);
			size_t end = HexStrToNum(info[L"end"]);
			
			old_spt.SetPos(swap_beg, Rut::RIO_BEGIN);
			swap_end = beg;
			size_t swap_size = swap_end - swap_beg;
			swap_buffer.SetSize(swap_size);
			old_spt.Read(swap_buffer.GetPtr(), swap_size);
			new_spt.Write(swap_buffer.GetPtr(), swap_size);
			swap_beg = end;


			std::wstring text = info[L"msg"];
			Rut::RxMem::Auto bin = this->MakeStruct(Rut::RxStr::ToMBCS(text, 936));
			new_spt.Write(bin.GetPtr(), bin.GetSize());
		}

		old_spt.SetPos(swap_beg, Rut::RIO_BEGIN);
		size_t swap_size = old_spt_size - swap_beg;
		swap_buffer.SetSize(swap_size);
		old_spt.Read(swap_buffer.GetPtr(), swap_size);
		new_spt.Write(swap_buffer.GetPtr(), swap_size);

	}
};


int main()
{
	SPT_Text_Editor spt_edit(L"0scene_pro001.spt");
	spt_edit.Insert();
	int a = 0;
}