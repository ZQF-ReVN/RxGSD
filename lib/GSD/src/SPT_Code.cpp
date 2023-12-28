#include "SPT_Code.h"
#include "SPT_Str.h"

#include <stdexcept>


namespace GSD::SPT
{
	Code::Code()
	{

	}

	void Code::Load(uint8_t* pData)
	{
		uint32_t* data_ptr = (uint32_t*)pData;

		m_uiCommand = data_ptr[0];
		m_uiVal_1 = data_ptr[1];
		m_uiVal_2 = data_ptr[2];
		m_uiVal_3 = data_ptr[3];
		m_uiVal_4 = data_ptr[4];
		m_uiSequnece = data_ptr[5];
		m_uiArgType1Count = data_ptr[6];
		m_uiArgType2Count = data_ptr[7];
		m_uiArgType3Count = data_ptr[8];

		uint8_t* append_data_ptr = pData + 9 * 4;

		if (m_uiCommand == 1) // Proc Text Struct
		{
			m_ArgType0.Load(append_data_ptr);
			append_data_ptr += m_ArgType0.GetSize();
		}

		for (size_t ite_type1 = 0; ite_type1 < m_uiArgType1Count; ite_type1++)
		{
			Arg_Type1 type1;
			type1.Load(append_data_ptr);
			append_data_ptr += type1.GetSize();
			m_vcArgType1.push_back(type1);
		}

		for (size_t ite_type2 = 0; ite_type2 < m_uiArgType2Count; ite_type2++)
		{
			Arg_Type2 type2;
			type2.Load(append_data_ptr);
			append_data_ptr += type2.GetSize();
			m_vcArgType2.push_back(type2);
		}

		for (size_t ite_type3 = 0; ite_type3 < m_uiArgType3Count; ite_type3++)
		{
			Arg_Type3 type3;
			type3.Load(append_data_ptr);
			append_data_ptr += type3.GetSize();
			m_vcArgType3.push_back(type3);
		}
	}

	Rut::RxMem::Auto Code::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		{
			uint32_t* tmp_ptr = (uint32_t*)cur_ptr;
			tmp_ptr[0] = m_uiCommand;
			tmp_ptr[1] = m_uiVal_1;
			tmp_ptr[2] = m_uiVal_2;
			tmp_ptr[3] = m_uiVal_3;
			tmp_ptr[4] = m_uiVal_4;
			tmp_ptr[5] = m_uiSequnece;
			tmp_ptr[6] = m_uiArgType1Count;
			tmp_ptr[7] = m_uiArgType2Count;
			tmp_ptr[8] = m_uiArgType3Count;
		}
		cur_ptr += 9 * 4;


		if (m_uiCommand == 1) // Proc Text Struct
		{
			Rut::RxMem::Auto type0_mem = m_ArgType0.Make();
			memcpy(cur_ptr, type0_mem.GetPtr(), type0_mem.GetSize());
			cur_ptr += type0_mem.GetSize();
		}

		for (auto& type1 : m_vcArgType1)
		{
			Rut::RxMem::Auto type1_mem = type1.Make();
			memcpy(cur_ptr, type1_mem.GetPtr(), type1_mem.GetSize());
			cur_ptr += type1_mem.GetSize();
		}

		for (auto& type2 : m_vcArgType2)
		{
			Rut::RxMem::Auto type2_mem = type2.Make();
			memcpy(cur_ptr, type2_mem.GetPtr(), type2_mem.GetSize());
			cur_ptr += type2_mem.GetSize();
		}

		for (auto& type3 : m_vcArgType3)
		{
			Rut::RxMem::Auto type3_mem = type3.Make();
			memcpy(cur_ptr, type3_mem.GetPtr(), type3_mem.GetSize());
			cur_ptr += type3_mem.GetSize();
		}

		return mem_data;
	}

	Rut::RxJson::JValue Code::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		json[L"Command"] = Str::NumToStr(L"0x%08x", m_uiCommand);
		json[L"Val_1"] = Str::NumToStr(L"0x%08x", m_uiVal_1);
		json[L"Val_2"] = Str::NumToStr(L"0x%08x", m_uiVal_2);
		json[L"Val_3"] = Str::NumToStr(L"0x%08x", m_uiVal_3);
		json[L"Val_4"] = Str::NumToStr(L"0x%08x", m_uiVal_4);
		json[L"Sequnece"] = Str::NumToStr(L"0x%08x", m_uiSequnece);
		json[L"ArgType1Count"] = Str::NumToStr(L"0x%08x", m_uiArgType1Count);
		json[L"ArgType2Count"] = Str::NumToStr(L"0x%08x", m_uiArgType2Count);
		json[L"ArgType3Count"] = Str::NumToStr(L"0x%08x", m_uiArgType3Count);

		Rut::RxJson::JValue& json_type0 = json[L"ArgType0"];
		Rut::RxJson::JValue& json_type1 = json[L"ArgType1"];
		Rut::RxJson::JValue& json_type2 = json[L"ArgType2"];
		Rut::RxJson::JValue& json_type3 = json[L"ArgType3"];

		if (m_uiCommand == 1) // Proc Text Struct
		{
			json_type0 = m_ArgType0.Make(nCodePage);
		}

		for (auto& type1 : m_vcArgType1)
		{
			json_type1.Append(type1.Make(nCodePage));
		}

		for (auto& type2 : m_vcArgType2)
		{
			json_type2.Append(type2.Make(nCodePage));
		}

		for (auto& type3 : m_vcArgType3)
		{
			json_type3.Append(type3.Make(nCodePage));
		}

		return json;
	}

	size_t Code::GetSize() const
	{
		size_t code_size = 0;

		code_size += sizeof(m_uiCommand);
		code_size += sizeof(m_uiVal_1);
		code_size += sizeof(m_uiVal_2);
		code_size += sizeof(m_uiVal_3);
		code_size += sizeof(m_uiVal_4);
		code_size += sizeof(m_uiSequnece);
		code_size += sizeof(m_uiArgType1Count);
		code_size += sizeof(m_uiArgType2Count);
		code_size += sizeof(m_uiArgType3Count);

		if (m_uiCommand == 1)
		{
			code_size += m_ArgType0.GetSize();
		}

		for (auto& type1 : m_vcArgType1)
		{
			code_size += type1.GetSize();
		}

		for (auto& type2 : m_vcArgType2)
		{
			code_size += type2.GetSize();
		}

		for (auto& type3 : m_vcArgType3)
		{
			code_size += type3.GetSize();
		}

		return code_size;
	}

	size_t Code::GetCommand() const
	{
		return m_uiCommand;
	}

	Arg_Type0& Code::GetArgType0()
	{
		return m_ArgType0;
	}
}