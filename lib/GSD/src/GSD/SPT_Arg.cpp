#include "SPT_Arg.h"
#include "SPT_Str.h"

#include <ranges>
#include <stdexcept>


namespace GSD::SPT
{
	Arg_Type0::Arg_Type0()
	{

	}

	Arg_Type0::Arg_Type0(Rut::RxMem::View& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type0::Arg_Type0(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	void Arg_Type0::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiNameReallySeq >> m_uiNameDisplaySeq >> m_uiUn2 >> m_uiVoiceFileSeq >> m_uiStrType0Len >> m_uiStrType1Len >> m_uiStrType2Len;

		if (m_uiStrType0Len)
		{
			for (auto i : std::views::iota(0u, m_uiStrType0Len)) 
			{ 
				m_vcStrType0.emplace_back(vMem.Read<SPT_Char_Entry>());
			}
		}

		if (m_uiStrType1Len)
		{
			m_msStrType1 = { vMem.CurPtr<char>() };
			vMem.Skip(m_uiStrType1Len + 1);
		}

		if (m_uiStrType2Len)
		{
			m_msStrType2 = { vMem.CurPtr<char>() };
			vMem.Skip(m_uiStrType2Len + 1);
		}
	}

	void Arg_Type0::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiNameReallySeq = Str::StrToNum(L"0x%08x", rfJson[L"NameReallySeq"]);
		m_uiNameDisplaySeq = Str::StrToNum(L"0x%08x", rfJson[L"NameDisplaySeq"]);
		m_uiUn2 = Str::StrToNum(L"0x%08x", rfJson[L"Un2"]);
		m_uiVoiceFileSeq = Str::StrToNum(L"0x%08x", rfJson[L"VoiceFileSeq"]);
		this->SetType0Text(Str::MakeANSI(rfJson[L"StrType0"], nCodePage));
		this->SetType1Text(Str::MakeANSI(rfJson[L"StrType1"], nCodePage));
		this->SetType2Text(Str::MakeANSI(rfJson[L"StrType2"], nCodePage));
	}

	void Arg_Type0::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());

		Rut::RxMem::View view = amMem.GetView();

		view << m_uiNameReallySeq << m_uiNameDisplaySeq << m_uiUn2 << m_uiVoiceFileSeq << m_uiStrType0Len << m_uiStrType1Len << m_uiStrType2Len;

		if (m_uiStrType0Len)
		{
			for (auto& entry : m_vcStrType0)
			{
				view << entry;
			}
		}

		if (m_uiStrType1Len)
		{
			view.Write(m_msStrType1.data(), m_uiStrType1Len + 1);
		}

		if (m_uiStrType2Len)
		{
			view.Write(m_msStrType2.data(), m_uiStrType2Len + 1);
		}
	}

	void Arg_Type0::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"NameReallySeq"] = Str::NumToStr(L"0x%08x", m_uiNameReallySeq);
		rfJson[L"NameDisplaySeq"] = Str::NumToStr(L"0x%08x", m_uiNameDisplaySeq);
		rfJson[L"Un2"] = Str::NumToStr(L"0x%08x", m_uiUn2);
		rfJson[L"VoiceFileSeq"] = Str::NumToStr(L"0x%08x", m_uiVoiceFileSeq);
		rfJson[L"StrType0"] = Str::LoadANSI(this->GetType0Text(), nCodePage);
		rfJson[L"StrType1"] = Str::LoadANSI(m_msStrType1, nCodePage);
		rfJson[L"StrType2"] = Str::LoadANSI(m_msStrType2, nCodePage);
	}

	void Arg_Type0::SetType0Text(const std::string& msText)
	{
		m_vcStrType0.clear();
		m_vcStrType0 = Str::MakeCharTable(msText);
		m_uiStrType0Len = (uint32_t)m_vcStrType0.size();
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
		return Str::ParseCharTable(m_vcStrType0);
	}

	std::wstring Arg_Type0::GetType0Text(size_t nCodePage) const
	{
		return Str::LoadANSI(this->GetType0Text(), nCodePage);
	}

	size_t Arg_Type0::GetNameSeq() const
	{
		return (size_t)m_uiNameReallySeq;
	}

	size_t Arg_Type0::GetSize() const
	{
		size_t size = 0;
		size += sizeof(m_uiNameReallySeq) + sizeof(m_uiNameDisplaySeq) + sizeof(m_uiUn2) + sizeof(m_uiVoiceFileSeq);
		size += sizeof(m_uiStrType0Len) + sizeof(m_uiStrType1Len) + sizeof(m_uiStrType2Len);
		size += m_vcStrType0.size() * sizeof(SPT_Char_Entry);
		size += m_uiStrType1Len ? (m_uiStrType1Len + 1) : 0;
		size += m_uiStrType2Len ? (m_uiStrType2Len + 1) : 0;
		return size;
	}


	Arg_Type1::Arg_Type1()
	{

	}

	Arg_Type1::Arg_Type1(Rut::RxMem::View& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type1::Arg_Type1(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	void Arg_Type1::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiVal_0 >> m_uiVal_1 >> m_uiVal_2 >> m_uiVal_3 >> m_uiStrLen >> m_uiVal_5 >> m_ucVal_6 >> m_uiVal_7;

		if (m_uiStrLen)
		{
			m_msStr = { vMem.CurPtr<char>(), m_uiStrLen };
			vMem.Skip(m_uiStrLen + 1);
		}
	}

	void Arg_Type1::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiVal_0 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_0"]);
		m_uiVal_1 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_1"]);
		m_uiVal_2 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_2"]);
		m_uiVal_3 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_3"]);
		m_uiVal_5 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_5"]);
		m_ucVal_6 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_6"]);
		m_uiVal_7 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_7"]);
		this->SetStr(Str::MakeANSI(rfJson[L"Str"], nCodePage));
	}

	void Arg_Type1::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());

		Rut::RxMem::View view = amMem.GetView();

		view << m_uiVal_0 << m_uiVal_1 << m_uiVal_2 << m_uiVal_3 << m_uiStrLen << m_uiVal_5 << m_ucVal_6 << m_uiVal_7;

		m_uiStrLen ? (void)view.Write(m_msStr.data(), m_uiStrLen + 1) : void(0);
	}

	void Arg_Type1::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"Val_0"] = Str::NumToStr(L"0x%08x", m_uiVal_0);
		rfJson[L"Val_1"] = Str::NumToStr(L"0x%08x", m_uiVal_1);
		rfJson[L"Val_2"] = Str::NumToStr(L"0x%08x", m_uiVal_2);
		rfJson[L"Val_3"] = Str::NumToStr(L"0x%08x", m_uiVal_3);
		rfJson[L"Val_5"] = Str::NumToStr(L"0x%08x", m_uiVal_5);
		rfJson[L"Val_6"] = Str::NumToStr(L"0x%08x", m_ucVal_6);
		rfJson[L"Val_7"] = Str::NumToStr(L"0x%08x", m_uiVal_7);
		rfJson[L"Str"] = Str::LoadANSI(m_msStr, nCodePage);
	}

	size_t Arg_Type1::GetSize() const
	{
		size_t size = 0;
		size += sizeof(m_uiVal_0) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2) + sizeof(m_uiVal_3);
		size += sizeof(m_uiStrLen) + sizeof(m_uiVal_5) + sizeof(m_ucVal_6) + sizeof(m_uiVal_7);
		size += m_uiStrLen ? (m_uiStrLen + 1) : 0;
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

	Arg_Type2::Arg_Type2(Rut::RxMem::View& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type2::Arg_Type2(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	void Arg_Type2::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiParameterType1Count;

		for (auto i : std::views::iota(0u, m_uiParameterType1Count))
		{
			m_vcParameterType1.emplace_back(vMem);
		}
	}

	void Arg_Type2::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiParameterType1Count = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"ArgType1Count"]);

		for (auto& type1_json : rfJson[L"ArgType1List"].ToAry()) 
		{ 
			m_vcParameterType1.emplace_back(type1_json, nCodePage);
		}
	}

	void Arg_Type2::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());

		Rut::RxMem::View view = amMem.GetView();

		view << m_uiParameterType1Count;

		Rut::RxMem::Auto buffer;
		for (auto& type1: m_vcParameterType1) 
		{ 
			type1.Make(buffer);
			view << buffer;
		}
	}

	void Arg_Type2::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"ArgType1Count"] = Str::NumToStr(L"0x%08x", m_uiParameterType1Count);

		Rut::RxJson::JArray& count_arrary = rfJson[L"ArgType1List"].ToAry();
		count_arrary.reserve(m_vcParameterType1.size());
		for (auto& type1 : m_vcParameterType1) 
		{
			Rut::RxJson::JValue tmp;
			type1.Make(tmp, nCodePage);
			count_arrary.push_back(std::move(tmp));
		}
	}

	size_t Arg_Type2::GetSize() const
	{
		size_t size = sizeof(m_uiParameterType1Count);

		for (auto& type1 : m_vcParameterType1) 
		{ 
			size += type1.GetSize();
		}

		return size;
	}


	Arg_Type3::Arg_Type3()
	{

	}

	Arg_Type3::Arg_Type3(Rut::RxMem::View& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type3::Arg_Type3(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	void Arg_Type3::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiVal_0 >> m_uiVal_1 >> m_uiVal_2;
	}

	void Arg_Type3::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiVal_0 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_0"]);
		m_uiVal_1 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_1"]);
		m_uiVal_2 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Val_2"]);
	}

	void Arg_Type3::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());
		amMem.GetView() << m_uiVal_0 << m_uiVal_1 << m_uiVal_2;
	}

	void Arg_Type3::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"Val_0"] = Str::NumToStr(L"0x%08x", m_uiVal_0);
		rfJson[L"Val_1"] = Str::NumToStr(L"0x%08x", m_uiVal_1);
		rfJson[L"Val_2"] = Str::NumToStr(L"0x%08x", m_uiVal_2);
	}

	size_t Arg_Type3::GetSize() const
	{
		return sizeof(m_uiVal_0) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2);
	}

}