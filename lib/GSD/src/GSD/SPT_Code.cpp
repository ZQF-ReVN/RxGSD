#include "SPT_Code.h"
#include "SPT_Str.h"

#include <ranges>
#include <stdexcept>


namespace GSD::SPT
{
	Code::Code()
	{

	}

	Code::Code(Rut::RxMem::View& vMem)
	{
		this->Load(vMem);
	}

	Code::Code(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	void Code::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiCommand >> m_uiVal_1 >> m_uiVal_2 >> m_uiVal_3 >> m_uiVal_4 >> m_uiSequnece >> m_uiArgType1Count >> m_uiArgType2Count >> m_uiArgType3Count;

		m_uiCommand == 1 ? (void)m_ArgType0.Load(vMem) : void(0);// Proc Text Struct

		for (auto i : std::views::iota(0u, m_uiArgType1Count)) 
		{ 
			m_vcArgType1.emplace_back(vMem);
		}

		for (auto i : std::views::iota(0u, m_uiArgType2Count)) 
		{ 
			m_vcArgType2.emplace_back(vMem);
		}

		for (auto i : std::views::iota(0u, m_uiArgType3Count)) 
		{ 
			m_vcArgType3.emplace_back(vMem);
		}
	}

	void Code::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiCommand = Str::StrToNum(L"0x%08x", rfJson[L"Command"]);
		m_uiVal_1 = Str::StrToNum(L"0x%08x", rfJson[L"Val_1"]);
		m_uiVal_2 = Str::StrToNum(L"0x%08x", rfJson[L"Val_2"]);
		m_uiVal_3 = Str::StrToNum(L"0x%08x", rfJson[L"Val_3"]);
		m_uiVal_4 = Str::StrToNum(L"0x%08x", rfJson[L"Val_4"]);
		m_uiSequnece = Str::StrToNum(L"0x%08x", rfJson[L"Sequnece"]);
		m_uiArgType1Count = Str::StrToNum(L"0x%08x", rfJson[L"ArgType1Count"]);
		m_uiArgType2Count = Str::StrToNum(L"0x%08x", rfJson[L"ArgType2Count"]);
		m_uiArgType3Count = Str::StrToNum(L"0x%08x", rfJson[L"ArgType3Count"]);

		m_uiCommand == 1 ? (void)m_ArgType0.Load(rfJson[L"ArgType0"], nCodePage) : void(0); // Proc Text Struct

		for (auto& type1_json : rfJson[L"ArgType1"].ToAry()) 
		{ 
			m_vcArgType1.emplace_back(type1_json, nCodePage);
		}

		for (auto& type2_json : rfJson[L"ArgType2"].ToAry()) 
		{ 
			m_vcArgType2.emplace_back(type2_json, nCodePage);
		}

		for (auto& type3_json : rfJson[L"ArgType3"].ToAry()) 
		{ 
			m_vcArgType3.emplace_back(type3_json, nCodePage);
		}
	}

	Rut::RxMem::Auto Code::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		Rut::RxMem::View view = mem_data.GetView();

		view << m_uiCommand << m_uiVal_1 << m_uiVal_2 << m_uiVal_3 << m_uiVal_4 << m_uiSequnece << m_uiArgType1Count << m_uiArgType2Count << m_uiArgType3Count;

		m_uiCommand == 1 ? (void)(view << m_ArgType0.Make()) : void(0); // Proc Text Struct

		for (auto& type1 : m_vcArgType1) 
		{ 
			view << type1.Make();
		}

		for (auto& type2 : m_vcArgType2) 
		{ 
			view << type2.Make();
		}

		for (auto& type3 : m_vcArgType3) 
		{ 
			view << type3.Make();
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

		if (m_uiCommand == 1)// Proc Text Struct
		{
			json[L"ArgType0"] = m_ArgType0.Make(nCodePage);
		}

		if (m_vcArgType1.size())
		{
			Rut::RxJson::JArray& json_type1 = json[L"ArgType1"].ToAry();
			for (auto& type1 : m_vcArgType1)
			{
				json_type1.emplace_back(type1.Make(nCodePage));
			}
		}

		if (m_vcArgType2.size())
		{
			Rut::RxJson::JArray& json_type2 = json[L"ArgType2"].ToAry();
			for (auto& type2 : m_vcArgType2)
			{
				json_type2.emplace_back(type2.Make(nCodePage));
			}
		}

		if (m_vcArgType3.size())
		{
			Rut::RxJson::JArray& json_type3 = json[L"ArgType3"].ToAry();
			for (auto& type3 : m_vcArgType3)
			{
				json_type3.emplace_back(type3.Make(nCodePage));
			}
		}

		return json;
	}

	size_t Code::GetSize() const
	{
		size_t code_size = 0;

		code_size += sizeof(m_uiCommand) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2) + sizeof(m_uiVal_3) + sizeof(m_uiVal_4);
		code_size += sizeof(m_uiSequnece) + sizeof(m_uiArgType1Count) + sizeof(m_uiArgType2Count) + sizeof(m_uiArgType3Count);
		code_size += m_uiCommand == 1 ? m_ArgType0.GetSize() : 0;

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