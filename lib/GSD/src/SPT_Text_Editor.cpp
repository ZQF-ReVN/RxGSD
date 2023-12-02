#include "SPT_Text_Editor.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxJson.h"

#include <list>


namespace GSD::SPT
{
	std::string Text_Editor::NumToHexStr(const size_t nValue)
	{
		char buf[64];
		(void)sprintf_s(buf, 64, "0x%08X", nValue);
		return buf;
	}

	std::wstring Text_Editor::NumToHexWStr(const size_t nValue)
	{
		wchar_t buf[64];
		(void)swprintf_s(buf, 64, L"0x%08X", nValue);
		return buf;
	}

	size_t Text_Editor::HexWStrToNum(const std::wstring_view& wsText)
	{
		size_t num = 0;
		(void)swscanf_s(wsText.data(), L"0x%08X", &num);
		return num;
	}

	bool Text_Editor::IsDBCS(uint32_t uiChar)
	{
		return uiChar >= 0x81 ? true : false;
	}

	void Text_Editor::DBCSReadChar(std::string& msText, uint32_t uiChar)
	{
		char dbcs_char_l = (char)((uiChar >> 0) & 0x00FF);
		char dbcs_char_h = (char)((uiChar >> 8) & 0x00FF);
		((uint8_t)dbcs_char_l >= 0x81u) ? (void)(msText.append(1, dbcs_char_l), msText.append(1, dbcs_char_h)) : (void)(msText.append(1, dbcs_char_l));
	}

	void CheckEntry(const SPT_Char_Entry& rfEntry)
	{
		switch (rfEntry.uiType)
		{
		// normal char flag
		case 0x7:
		{
			if (rfEntry.uiNotationCount == 0)
			{
				return;
			}
		}
		break;

		// notation beg flag
		case 0x8:
		{
			if (rfEntry.uiChar == 0)
			{
				return;
			}
		}
		break;

		case 0x9:
		{
			if ((rfEntry.uiNotationCount == 0) && (rfEntry.uiChar == 0))
			{
				return;
			}
		}
		break;

		case 0xD:
		{
			//if ((rfEntry.uiNotationCount == 0) && (rfEntry.uiChar == 0))
			//{
			//	return;
			//}
			return;
		}
		break;
		}

		throw std::runtime_error("Error Char Type!");
	}

	std::wstring Text_Editor::ReadText(SPT_Text_Entry* pEntry, size_t uiCodePage)
	{
		std::vector<uint32_t> char_list;
		{
			SPT_Char_Entry* char_entry_arry = (SPT_Char_Entry*)(pEntry + 1);
			for (size_t ite_entry = 0; ite_entry < pEntry->uiCharCount; ite_entry++)
			{
				const SPT_Char_Entry& char_entry = char_entry_arry[ite_entry];

				CheckEntry(char_entry);

				switch (char_entry.uiType)
				{
				// normal char flag
				case 0x7:
				{
					char_list.push_back(char_entry.uiChar);
				}
				break;

				// notation beg flag
				case 0x8:
				{
					char_list.insert(char_list.end() - char_entry.uiNotationCount, '<');
					char_list.push_back('/');
				}
				break;

				// notation end flag
				case 0x9:
				{
					char_list.push_back('>');
				}
				break;

				// string end flag
				case 0xD:
				{
					// Nothing
				}
				break;

				default: 
				{
					throw std::runtime_error("Unknow Char Type!");
				}
				}
			}
		}

		std::string text;
		for (auto charx : char_list)
		{
			DBCSReadChar(text, charx);
		}

		return Rut::RxStr::ToWCS(text, uiCodePage);
	}

	void Text_Editor::MakeStruct(Rut::RxMem::Auto& rfMem, const std::string& msText)
	{
		std::vector<SPT_Char_Entry> char_entry_list;
		{
			size_t notation_pos_beg = 0;
			SPT_Char_Entry entry = { 0 };
			for (size_t ite_char = 0; ite_char < msText.size(); ite_char++)
			{
				if (IsDBCS(msText[ite_char]))
				{
					uint32_t dbcs_char_l = (msText[ite_char + 0]) & 0xFF;
					uint32_t dbcs_char_h = (msText[ite_char + 1]) & 0xFF;
					ite_char++;
					entry.uiType = 0x7;
					entry.uiNotationCount = 0x0;
					entry.uiChar = (dbcs_char_l) | (dbcs_char_h << 0x8);
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
						if (notation_pos_beg == 0) { throw std::runtime_error("Notation token mismatched"); }

						entry.uiType = 0x8; // notation beg
						entry.uiNotationCount = char_entry_list.size() - notation_pos_beg;
						entry.uiChar = 0;

						notation_pos_beg = 0;
					}
					else if (msText[ite_char] == '>')
					{
						entry.uiType = 0x9; // notation end
						entry.uiNotationCount = 0;
						entry.uiChar = 0;
					}
					else
					{
						entry.uiType = 0x7; // normal char
						entry.uiNotationCount = 0x0;
						entry.uiChar = msText[ite_char];
					}
				}

				char_entry_list.push_back(entry);
			}

			entry.uiType = 0xD; // end flag
			entry.uiNotationCount = 0x0;
			entry.uiChar = 0;
			char_entry_list.push_back(entry);
		}


		rfMem.SetSize(sizeof(SPT_Text_Entry) + char_entry_list.size() * sizeof(SPT_Char_Entry));

		SPT_Text_Entry* text_entry_ptr = (SPT_Text_Entry*)rfMem.GetPtr();
		text_entry_ptr->uiCharCount = char_entry_list.size();
		text_entry_ptr->aUn0[0] = 0;
		text_entry_ptr->aUn0[1] = 0;

		SPT_Char_Entry* char_entry_arry = (SPT_Char_Entry*)(text_entry_ptr + 1);

		for (size_t ite_entry = 0; ite_entry < char_entry_list.size(); ite_entry++)
		{
			char_entry_arry[ite_entry] = char_entry_list[ite_entry];
		}
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

		size_t search_size = spt_size - sizeof(text_code);

		for (size_t ite_bytes = 0; ite_bytes < search_size; ite_bytes++)
		{
			uint8_t* cur_ptr = spt_ptr + ite_bytes;
			if (memcmp(cur_ptr, text_code, sizeof(text_code)) == 0)
			{
				SPT_Text_Entry* spt_text_ptr = (SPT_Text_Entry*)(cur_ptr + sizeof(text_code));
				SPT_Char_Entry* spt_char_last_ptr = (SPT_Char_Entry*)((size_t)spt_text_ptr + sizeof(SPT_Text_Entry) + (spt_text_ptr->uiCharCount - 1) * sizeof(SPT_Char_Entry));
				if ((spt_text_ptr->uiCharCount != 0) &&
					(spt_text_ptr->aUn0[0] == 0) &&
					(spt_text_ptr->aUn0[1] == 0) &&
					(spt_char_last_ptr->uiType == 0xD))
				{
					this->m_vcTextEntryPtr.push_back(spt_text_ptr);
				}
			}
		}
	}

	void Text_Editor::Extract(const std::wstring_view wsJson, size_t uiCodePage)
	{
		this->ReadEntry();

		if (this->m_vcTextEntryPtr.empty()) { return; }

		Rut::RxJson::Value json;
		uint8_t* spt_ptr = this->m_amSPT.GetPtr();

		for (SPT_Text_Entry* entry_ptr : this->m_vcTextEntryPtr)
		{
			size_t beg_foa = (size_t)((uint8_t*)entry_ptr - spt_ptr);
			size_t end_foa = beg_foa + sizeof(SPT_Text_Entry) + entry_ptr->uiCharCount * sizeof(SPT_Char_Entry);

			Rut::RxJson::JObject text_obj;
			text_obj[L"beg"] = NumToHexWStr(beg_foa);
			text_obj[L"end"] = NumToHexWStr(end_foa);
			text_obj[L"msg"] = ReadText(entry_ptr, uiCodePage);

			json.Append(std::move(text_obj));
		}

		Rut::RxJson::Parser::Save(json, wsJson);
	}

	void Text_Editor::Insert(const std::wstring_view wsJson, size_t uiCodePage)
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
			size_t text_beg = HexWStrToNum(info[L"beg"]);
			size_t text_end = HexWStrToNum(info[L"end"]);

			FileSwapData(old_spt, swap_beg, text_beg, new_spt, tmp_buffer);
			swap_beg = text_end;

			MakeStruct(tmp_buffer, Rut::RxStr::ToMBCS(text, uiCodePage));
			new_spt.Write(tmp_buffer.GetPtr(), tmp_buffer.GetSize());
		}

		FileSwapData(old_spt, swap_beg, swap_max_end, new_spt, tmp_buffer);
	}
}