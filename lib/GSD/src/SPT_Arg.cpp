#include "SPT_Arg.h"
#include "SPT_Str.h"

#include <stdexcept>


namespace GSD::SPT
{
	Arg_Type0::Arg_Type0()
	{

	}

	void Arg_Type0::Load(uint8_t* pData)
	{
		uint8_t* cur_prt = pData;

		{
			uint32_t* tmp_ptr = (uint32_t*)pData;
			m_uiNameSeq0 = tmp_ptr[0];
			m_uiNameSeq1 = tmp_ptr[1];
			m_uiUn2 = tmp_ptr[2];
			m_uiUn3 = tmp_ptr[3];
			m_uiStrType0Len = tmp_ptr[4];
			m_uiStrType1Len = tmp_ptr[5];
			m_uiStrType2Len = tmp_ptr[6];
		}
		cur_prt += 4 * 7;

		if (m_uiStrType0Len)
		{
			const SPT_Char_Entry* char_entry_arry = (SPT_Char_Entry*)cur_prt;
			for (size_t ite_entry = 0; ite_entry < m_uiStrType0Len; ite_entry++)
			{
				SPT_Char_Entry entry = char_entry_arry[ite_entry];
				m_vcStrType0CharList.push_back(entry);
			}

			cur_prt += m_vcStrType0CharList.size() * sizeof(SPT_Char_Entry);
		}
		if (m_uiStrType1Len)
		{
			m_msStrType1 = { (char*)cur_prt };
			cur_prt += m_uiStrType1Len + 1;
		}
		if (m_uiStrType2Len)
		{
			m_msStrType2 = { (char*)cur_prt };
			cur_prt += m_uiStrType1Len + 1;
		}
	}

	void Arg_Type0::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiNameSeq0 = Str::StrToNum(L"0x%08x", rfJson[L"NameSeq0"]);
		m_uiNameSeq1 = Str::StrToNum(L"0x%08x", rfJson[L"NameSeq1"]);
		m_uiUn2 = Str::StrToNum(L"0x%08x", rfJson[L"Un2"]);
		m_uiUn3 = Str::StrToNum(L"0x%08x", rfJson[L"Un3"]);
		this->SetType0Text(Str::MakeANSI(rfJson[L"StrType0"],nCodePage));
		this->SetType1Text(Str::MakeANSI(rfJson[L"StrType1"], nCodePage));
		this->SetType2Text(Str::MakeANSI(rfJson[L"StrType2"], nCodePage));
	}

	Rut::RxMem::Auto Arg_Type0::Make() const
	{
		Rut::RxMem::Auto make_mem(this->GetSize());
		uint8_t* cur_prt = make_mem.GetPtr();

		{
			uint32_t* tmp_ptr = (uint32_t*)cur_prt;
			tmp_ptr[0] = m_uiNameSeq0;
			tmp_ptr[1] = m_uiNameSeq1;
			tmp_ptr[2] = m_uiUn2;
			tmp_ptr[3] = m_uiUn3;
			tmp_ptr[4] = m_uiStrType0Len;
			tmp_ptr[5] = m_uiStrType1Len;
			tmp_ptr[6] = m_uiStrType2Len;
		}
		cur_prt += 4 * 7;

		if (m_uiStrType0Len)
		{
			SPT_Char_Entry* char_entry_arry = (SPT_Char_Entry*)cur_prt;
			for (size_t ite_entry = 0; ite_entry < m_uiStrType0Len; ite_entry++)
			{
				char_entry_arry[ite_entry] = m_vcStrType0CharList[ite_entry];
			}

			cur_prt += m_vcStrType0CharList.size() * sizeof(SPT_Char_Entry);
		}
		if (m_uiStrType1Len)
		{
			memcpy(cur_prt, m_msStrType1.data(), m_uiStrType1Len);
			cur_prt += m_uiStrType1Len + 1;
		}
		if (m_uiStrType2Len)
		{
			memcpy(cur_prt, m_msStrType2.data(), m_uiStrType2Len);
			cur_prt += m_uiStrType2Len + 1;
		}

		return make_mem;
	}

	Rut::RxJson::JValue Arg_Type0::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"NameSeq0"] = Str::NumToStr(L"0x%08x", m_uiNameSeq0);
		json[L"NameSeq1"] = Str::NumToStr(L"0x%08x", m_uiNameSeq1);
		json[L"Un2"] = Str::NumToStr(L"0x%08x", m_uiUn2);
		json[L"Un3"] = Str::NumToStr(L"0x%08x", m_uiUn3);
		json[L"StrType0"] = Str::LoadANSI(this->GetType0Text(), nCodePage);
		json[L"StrType1"] = Str::LoadANSI(m_msStrType1, nCodePage);
		json[L"StrType2"] = Str::LoadANSI(m_msStrType2, nCodePage);
		return json;
	}

	void Arg_Type0::SetType0Text(const std::string& msText)
	{
		m_vcStrType0CharList.clear();
		m_vcStrType0CharList = Str::MakeCharTable(msText);
		m_uiStrType0Len = (uint32_t)m_vcStrType0CharList.size();
	}

	void Arg_Type0::SetType1Text(const std::string& msText)
	{
		m_msStrType1 = msText;
		m_uiStrType1Len = msText.size();
	}

	void Arg_Type0::SetType2Text(const std::string& msText)
	{
		m_msStrType2 = msText;
		m_uiStrType2Len = msText.size();
	}

	size_t Arg_Type0::GetType0TextLen()const
	{
		return m_uiStrType0Len;
	}

	std::string Arg_Type0::GetType0Text() const
	{
		return Str::ParseCharTable(m_vcStrType0CharList);
	}

	std::wstring Arg_Type0::GetType0Text(size_t nCodePage) const
	{
		return Str::LoadANSI(this->GetType0Text(), nCodePage);
	}

	size_t Arg_Type0::GetNameSeq() const
	{
		return (size_t)m_uiNameSeq0;
	}

	size_t Arg_Type0::GetSize() const
	{
		size_t size = 0;

		size += sizeof(m_uiNameSeq0) + sizeof(m_uiNameSeq1) + sizeof(m_uiUn2) + sizeof(m_uiUn3);
		size += sizeof(m_uiStrType0Len) + sizeof(m_uiStrType1Len) + sizeof(m_uiStrType2Len);
		size += m_vcStrType0CharList.size() * sizeof(SPT_Char_Entry);

		if (m_uiStrType1Len)
		{
			size += m_uiStrType1Len + 1;
		}
		if (m_uiStrType2Len)
		{
			size += m_uiStrType2Len + 1;
		}

		return size;
	}


	Arg_Type1::Arg_Type1()
	{

	}

	void Arg_Type1::Load(uint8_t* pData)
	{
		uint8_t* cur_ptr = pData;

		{
			uint32_t* tmp_ptr = (uint32_t*)cur_ptr;
			m_uiVal_0 = tmp_ptr[0];
			m_uiVal_1 = tmp_ptr[1];
			m_uiVal_2 = tmp_ptr[2];
			m_uiVal_3 = tmp_ptr[3];
			m_uiStrLen = tmp_ptr[4];
			m_uiVal_5 = tmp_ptr[5];
		}
		cur_ptr += 6 * 4;

		m_uiVal_6 = *(cur_ptr);
		cur_ptr += 1;

		m_uiVal_7 = *(uint32_t*)(cur_ptr);
		cur_ptr += 4;

		if (m_uiStrLen)
		{
			char* str_ptr = (char*)(cur_ptr);
			m_msStr = { str_ptr, m_uiStrLen };
		}
	}

	void Arg_Type1::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiVal_0 = Str::StrToNum(L"0x%08x", rfJson[L"Val_0"]);
		m_uiVal_1 = Str::StrToNum(L"0x%08x", rfJson[L"Val_1"]);
		m_uiVal_2 = Str::StrToNum(L"0x%08x", rfJson[L"Val_2"]);
		m_uiVal_3 = Str::StrToNum(L"0x%08x", rfJson[L"Val_3"]);
		m_uiVal_5 = Str::StrToNum(L"0x%08x", rfJson[L"Val_5"]);
		m_uiVal_6 = Str::StrToNum(L"0x%08x", rfJson[L"Val_6"]);
		m_uiVal_7 = Str::StrToNum(L"0x%08x", rfJson[L"Val_7"]);
		this->SetStr(Str::MakeANSI(rfJson[L"Str"], nCodePage));
	}

	Rut::RxMem::Auto Arg_Type1::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		{
			uint32_t* tmp_ptr = (uint32_t*)cur_ptr;
			tmp_ptr[0] = m_uiVal_0;
			tmp_ptr[1] = m_uiVal_1;
			tmp_ptr[2] = m_uiVal_2;
			tmp_ptr[3] = m_uiVal_3;
			tmp_ptr[4] = m_uiStrLen;
			tmp_ptr[5] = m_uiVal_5;
		}
		cur_ptr += 6 * 4;

		*(cur_ptr) = m_uiVal_6;
		cur_ptr += 1;

		*(uint32_t*)(cur_ptr) = m_uiVal_7;
		cur_ptr += 4;

		if (m_uiStrLen)
		{
			char* str_ptr = (char*)(cur_ptr);
			memcpy(str_ptr, m_msStr.data(), m_uiStrLen + 1);
		}

		return mem_data;
	}

	Rut::RxJson::JValue Arg_Type1::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Val_0"] = Str::NumToStr(L"0x%08x", m_uiVal_0);
		json[L"Val_1"] = Str::NumToStr(L"0x%08x", m_uiVal_1);
		json[L"Val_2"] = Str::NumToStr(L"0x%08x", m_uiVal_2);
		json[L"Val_3"] = Str::NumToStr(L"0x%08x", m_uiVal_3);
		json[L"Val_5"] = Str::NumToStr(L"0x%08x", m_uiVal_5);
		json[L"Val_6"] = Str::NumToStr(L"0x%08x", m_uiVal_6);
		json[L"Val_7"] = Str::NumToStr(L"0x%08x", m_uiVal_7);
		json[L"Str"] = Str::LoadANSI(m_msStr, nCodePage);
		return json;
	}

	size_t Arg_Type1::GetSize() const
	{
		size_t size = 0;

		size += sizeof(m_uiVal_0) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2) + sizeof(m_uiVal_3) + sizeof(m_uiStrLen) + sizeof(m_uiVal_5) + sizeof(m_uiVal_6) + sizeof(m_uiVal_7);

		if (m_uiStrLen)
		{
			size += m_uiStrLen + 1;
		}

		return size;
	}

	void Arg_Type1::SetStr(std::string_view msStr)
	{
		m_msStr = msStr;
		m_uiStrLen = msStr.size();
	}


	Arg_Type2::Arg_Type2()
	{

	}

	void Arg_Type2::Load(uint8_t* pData)
	{
		m_uiParameterType1Count = *(uint32_t*)pData;
		uint8_t* cur_ptr = pData + 4;

		for (size_t ite_type1 = 0; ite_type1 < m_uiParameterType1Count; ite_type1++)
		{
			Arg_Type1 type1;
			type1.Load(cur_ptr);
			cur_ptr += type1.GetSize();
			m_vcParameterType1.emplace_back(std::move(type1));
		}
	}

	Rut::RxMem::Auto Arg_Type2::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* data_ptr = mem_data.GetPtr();

		*(uint32_t*)data_ptr = m_uiParameterType1Count;
		uint8_t* cur_ptr = data_ptr + 4;

		for (const auto& type1 : m_vcParameterType1)
		{
			Rut::RxMem::Auto mem = type1.Make();
			memcpy(cur_ptr, mem.GetPtr(), mem.GetSize());
			cur_ptr += mem.GetSize();
		}

		return mem_data;
	}

	Rut::RxJson::JValue Arg_Type2::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"ArgType1Count"] = Str::NumToStr(L"0x%08x", m_uiParameterType1Count);
		Rut::RxJson::JValue& count_arrary = json[L"ArgType1List"];
		for (const auto& type1 : m_vcParameterType1)
		{
			count_arrary.Append(type1.Make(nCodePage));
		}
		return json;
	}

	size_t Arg_Type2::GetSize() const
	{
		size_t size = 0;

		size += sizeof(m_uiParameterType1Count);

		for (const auto& type1 : m_vcParameterType1)
		{
			size += type1.GetSize();
		}

		return size;
	}


	Arg_Type3::Arg_Type3()
	{

	}

	void Arg_Type3::Load(uint8_t* pData)
	{
		uint32_t* data_ptr = (uint32_t*)pData;
		m_uiVal_0 = data_ptr[0];
		m_uiVal_1 = data_ptr[1];
		m_uiVal_2 = data_ptr[2];
	}

	Rut::RxMem::Auto Arg_Type3::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint32_t* data_ptr = (uint32_t*)mem_data.GetPtr();

		data_ptr[0] = m_uiVal_0;
		data_ptr[1] = m_uiVal_1;
		data_ptr[2] = m_uiVal_2;

		return mem_data;
	}

	Rut::RxJson::JValue Arg_Type3::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Val_0"] = Str::NumToStr(L"0x%08x", m_uiVal_0);
		json[L"Val_1"] = Str::NumToStr(L"0x%08x", m_uiVal_1);
		json[L"Val_2"] = Str::NumToStr(L"0x%08x", m_uiVal_2);
		return json;
	}

	size_t Arg_Type3::GetSize() const
	{
		return sizeof(m_uiVal_0) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2);
	}

}