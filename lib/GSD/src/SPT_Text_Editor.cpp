#include "SPT_Text_Editor.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxJson.h"



namespace GSD::SPT
{
	std::wstring Text_Editor::NumToHexStr(const size_t nValue)
	{
		wchar_t foa_str[64];
		(void)swprintf_s(foa_str, 64, L"0x%08X", nValue);
		return foa_str;
	}

	size_t Text_Editor::HexStrToNum(const std::wstring_view& wsText)
	{
		size_t num = 0;
		(void)swscanf_s(wsText.data(), L"0x%08X", &num);
		return num;
	}

	size_t Text_Editor::DBCSCountChar(std::string_view msText)
	{
		size_t count = 0;
		for (auto ite_char = msText.begin(); ite_char != msText.end(); ite_char++)
		{
			(uint8_t)ite_char[0] >= 0x81 ? (void)(count++, ite_char++) : (void)(count++);
		}
		return count;
	}

	std::wstring Text_Editor::ReadText(SPT_Text_Entry* pEntry)
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

	void Text_Editor::MakeStruct(Rut::RxMem::Auto& rfMem, const std::string& msText)
	{
		size_t char_count = DBCSCountChar(msText);

		rfMem.SetSize(sizeof(SPT_Text_Entry) + (char_count + 1) * sizeof(SPT_Char_Entry));

		uint8_t* bin_ptr = rfMem.GetPtr();

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
	}

	void Text_Editor::FileSwapData(Rut::RxFile::Binary& fsSource, size_t nBegPos, size_t nEndPos, Rut::RxFile::Binary& fsDest, Rut::RxMem::Auto& rfBuffer)
	{
		size_t swap_size = nEndPos - nBegPos;
		rfBuffer.SetSize(swap_size);
		fsSource.SetPos(nBegPos, Rut::RIO_BEGIN);
		fsSource.Read(rfBuffer.GetPtr(), swap_size);
		fsDest.Write(rfBuffer.GetPtr(), swap_size);
	}


	Text_Editor::Text_Editor(std::wstring_view wsPath)
	{
		this->m_wsPath = wsPath;
		this->m_amSPT.LoadFile(wsPath);
	}

	void Text_Editor::ReadEntry()
	{
		this->m_vcTextEntryPtr.clear();

		uint8_t text_code[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

		uint8_t* spt_ptr = this->m_amSPT.GetPtr();
		size_t spt_size = this->m_amSPT.GetSize();

		for (size_t ite_bytes = 0; ite_bytes < spt_size; ite_bytes++)
		{
			uint8_t* cur_ptr = spt_ptr + ite_bytes;
			if (memcmp(cur_ptr, text_code, sizeof(text_code)) == 0)
			{
				SPT_Text_Entry* spt_text_ptr = (SPT_Text_Entry*)(cur_ptr + sizeof(text_code));
				SPT_Char_Entry* spt_char_last_ptr = (SPT_Char_Entry*)((size_t)spt_text_ptr + sizeof(SPT_Text_Entry) + (spt_text_ptr->uiCharCount - 1) * sizeof(SPT_Char_Entry));
				if ((spt_text_ptr->uiCharCount != 0) &&
					(spt_text_ptr->aUn0[0] == 0) &&
					(spt_text_ptr->aUn0[1] == 0) &&
					(spt_char_last_ptr->uiVal0 == 0xD))
				{
					this->m_vcTextEntryPtr.push_back(spt_text_ptr);
				}
			}
		}
	}

	void Text_Editor::Extract(const std::wstring_view wsJson)
	{
		this->ReadEntry();

		Rut::RxJson::Value json;
		uint8_t* spt_ptr = this->m_amSPT.GetPtr();

		for (SPT_Text_Entry* entry_ptr : this->m_vcTextEntryPtr)
		{
			size_t beg_foa = (size_t)((uint8_t*)entry_ptr - spt_ptr);
			size_t end_foa = beg_foa + sizeof(SPT_Text_Entry) + entry_ptr->uiCharCount * sizeof(SPT_Char_Entry);

			Rut::RxJson::JObject text_obj;
			text_obj[L"beg"] = NumToHexStr(beg_foa);
			text_obj[L"end"] = NumToHexStr(end_foa);
			text_obj[L"msg"] = ReadText(entry_ptr);

			json.Append(std::move(text_obj));
		}

		Rut::RxJson::Parser::Save(json, wsJson);
	}

	void Text_Editor::Insert(const std::wstring_view wsJson)
	{
		Rut::RxFile::Binary old_spt{ this->m_wsPath, Rut::RIO_READ };
		Rut::RxFile::Binary new_spt{ this->m_wsPath + L".new", Rut::RIO_WRITE };

		size_t swap_beg = 0;
		size_t swap_max_end = old_spt.GetSize();

		Rut::RxMem::Auto tmp_buffer;

		Rut::RxJson::Value json;
		Rut::RxJson::Parser().Load(wsJson, json);
		Rut::RxJson::JArray& info_list = json.ToAry();

		for (auto& info : info_list)
		{
			std::wstring text = info[L"msg"];
			size_t text_beg = HexStrToNum(info[L"beg"]);
			size_t text_end = HexStrToNum(info[L"end"]);

			FileSwapData(old_spt, swap_beg, text_beg, new_spt, tmp_buffer);
			swap_beg = text_end;

			MakeStruct(tmp_buffer, Rut::RxStr::ToMBCS(text, 936));
			new_spt.Write(tmp_buffer.GetPtr(), tmp_buffer.GetSize());
		}

		FileSwapData(old_spt, swap_beg, swap_max_end, new_spt, tmp_buffer);
	}
}