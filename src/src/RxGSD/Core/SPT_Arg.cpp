#include "SPT_Arg.h"
#include "SPT_Str.h"
#include <ranges>
#include <stdexcept>


namespace ZQF::RxGSD::SPT
{
	Arg_Type0::Arg_Type0()
	{

	}

	Arg_Type0::Arg_Type0(ZxMem& rfMem)
	{
		this->Load(rfMem);
	}

	Arg_Type0::Arg_Type0(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto Arg_Type0::Load(ZxMem& rfMem) -> void
	{
		rfMem >> m_uiNameReallySeq >> m_uiNameDisplaySeq >> m_uiUn2 >> m_uiVoiceFileSeq >> m_uiStrType0Len >> m_uiStrType1Len >> m_uiStrType2Len;

		if (m_uiStrType0Len)
		{
			for ([[maybe_unused]]auto idx : std::views::iota(0u, m_uiStrType0Len)) 
			{ 
				m_vcStrType0.emplace_back(rfMem.Get<SPT::Char_Entry>());
			}
		}

		if (m_uiStrType1Len)
		{
			m_msStrType1 = { rfMem.PtrCur<char*>() };
			rfMem.PosInc(static_cast<std::size_t>(m_uiStrType1Len) + 1);
		}

		if (m_uiStrType2Len)
		{
			m_msStrType2 = { rfMem.PtrCur<char*>() };
			rfMem.PosInc(static_cast<std::size_t>(m_uiStrType2Len) + 1);
		}
	}

	auto Arg_Type0::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_uiNameReallySeq = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("NameReallySeq").GetStrView());
		m_uiNameDisplaySeq = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("NameDisplaySeq").GetStrView());
		m_uiUn2 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un2").GetStrView());
		m_uiVoiceFileSeq = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("VoiceFileSeq").GetStrView());
		this->SetType0Text(Str::MakeANSI(rfJson.At("StrType0").GetStrView(), nCodePage));
		this->SetType1Text(Str::MakeANSI(rfJson.At("StrType1").GetStrView(), nCodePage));
		this->SetType2Text(Str::MakeANSI(rfJson.At("StrType2").GetStrView(), nCodePage));
	}

	auto Arg_Type0::Make(ZxMem& rfMem) const -> void
	{
		rfMem << m_uiNameReallySeq << m_uiNameDisplaySeq << m_uiUn2 << m_uiVoiceFileSeq << m_uiStrType0Len << m_uiStrType1Len << m_uiStrType2Len;

		if (m_uiStrType0Len)
		{
			for (const auto& entry : m_vcStrType0)
			{
				rfMem << entry;
			}
		}

		if (m_uiStrType1Len)
		{
			rfMem.Write(std::span{ m_msStrType1.data(), m_uiStrType1Len + 1 });
		}

		if (m_uiStrType2Len)
		{
			rfMem.Write(std::span{ m_msStrType2.data(), m_uiStrType2Len + 1 });
		}
	}

	auto Arg_Type0::Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void
	{
		rfJson["NameReallySeq"] = Str::NumToStr("0x%08x", m_uiNameReallySeq);
		rfJson["NameDisplaySeq"] = Str::NumToStr("0x%08x", m_uiNameDisplaySeq);
		rfJson["Un2"] = Str::NumToStr("0x%08x", m_uiUn2);
		rfJson["VoiceFileSeq"] = Str::NumToStr("0x%08x", m_uiVoiceFileSeq);
		rfJson["StrType0"] = Str::LoadANSI(this->GetType0Text(), nCodePage);
		rfJson["StrType1"] = Str::LoadANSI(m_msStrType1, nCodePage);
		rfJson["StrType2"] = Str::LoadANSI(m_msStrType2, nCodePage);
	}

	auto Arg_Type0::SetType0Text(const std::string_view msText) -> void
	{
		m_vcStrType0.clear();
		m_vcStrType0 = Str::MakeCharTable(msText);
		m_uiStrType0Len = static_cast<std::uint32_t>(m_vcStrType0.size());
	}

	auto Arg_Type0::SetType1Text(const std::string_view msText) -> void
	{
		m_msStrType1 = msText;
		m_uiStrType1Len = static_cast<std::int32_t>(msText.size());
	}

	auto Arg_Type0::SetType2Text(const std::string_view msText) -> void
	{
		m_msStrType2 = msText;
		m_uiStrType2Len = static_cast<std::int32_t>(msText.size());
	}

	auto Arg_Type0::GetType0TextLen() const -> std::size_t
	{
		return static_cast<std::size_t>(m_uiStrType0Len);
	}

	auto Arg_Type0::GetType0Text() const -> std::string
	{
		return Str::ParseCharTable(m_vcStrType0);
	}

	auto Arg_Type0::GetNameSeq() const -> std::size_t
	{
		return static_cast<std::size_t>(m_uiNameReallySeq);
	}

	auto Arg_Type0::SizeBytes() const -> std::size_t
	{
		std::size_t bytes{};
		bytes += sizeof(m_uiNameReallySeq) + sizeof(m_uiNameDisplaySeq) + sizeof(m_uiUn2) + sizeof(m_uiVoiceFileSeq);
		bytes += sizeof(m_uiStrType0Len) + sizeof(m_uiStrType1Len) + sizeof(m_uiStrType2Len);
		bytes += m_vcStrType0.size() * SPT::Char_Entry::SizeBytes;
		bytes += static_cast<std::size_t>(m_uiStrType1Len ? (m_uiStrType1Len + 1) : 0);
		bytes += static_cast<std::size_t>(m_uiStrType2Len ? (m_uiStrType2Len + 1) : 0);
		return bytes;
	}


	Arg_Type1::Arg_Type1()
	{

	}

	Arg_Type1::Arg_Type1(ZxMem& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type1::Arg_Type1(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto Arg_Type1::Load(ZxMem& vMem) -> void
	{
		vMem >> m_uiVal_0 >> m_uiVal_1 >> m_uiVal_2 >> m_uiVal_3 >> m_uiStrLen >> m_uiVal_5 >> m_ucVal_6 >> m_uiVal_7;

		if (m_uiStrLen)
		{
			m_msStr = { vMem.PtrCur<char*>(), m_uiStrLen };
			vMem.PosInc(static_cast<std::size_t>(m_uiStrLen) + 1);
		}
	}

	auto Arg_Type1::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_uiVal_0 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_0").GetStrView());
		m_uiVal_1 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_1").GetStrView());
		m_uiVal_2 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_2").GetStrView());
		m_uiVal_3 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_3").GetStrView());
		m_uiVal_5 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_5").GetStrView());
		m_ucVal_6 = Str::StrToNum<std::uint8_t>("0x%02x", rfJson.At("Val_6").GetStrView());
		m_uiVal_7 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_7").GetStrView());
		this->SetStr(Str::MakeANSI(rfJson.At("Str").GetStrView(), nCodePage));
	}

	auto Arg_Type1::Make(ZxMem& amMem) const -> void
	{
		amMem << m_uiVal_0 << m_uiVal_1 << m_uiVal_2 << m_uiVal_3 << m_uiStrLen << m_uiVal_5 << m_ucVal_6 << m_uiVal_7;

		m_uiStrLen ? (void)amMem.Write(std::span{ m_msStr.data(), m_uiStrLen + 1 }) : void(0);
	}

	auto Arg_Type1::Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void
	{
		rfJson["Val_0"] = Str::NumToStr("0x%08x", m_uiVal_0);
		rfJson["Val_1"] = Str::NumToStr("0x%08x", m_uiVal_1);
		rfJson["Val_2"] = Str::NumToStr("0x%08x", m_uiVal_2);
		rfJson["Val_3"] = Str::NumToStr("0x%08x", m_uiVal_3);
		rfJson["Val_5"] = Str::NumToStr("0x%08x", m_uiVal_5);
		rfJson["Val_6"] = Str::NumToStr("0x%02x", m_ucVal_6);
		rfJson["Val_7"] = Str::NumToStr("0x%08x", m_uiVal_7);
		rfJson["Str"] = Str::LoadANSI(m_msStr, nCodePage);
	}

	auto Arg_Type1::SizeBytes() const -> std::size_t
	{
		std::size_t bytes{};
		bytes += sizeof(m_uiVal_0) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2) + sizeof(m_uiVal_3);
		bytes += sizeof(m_uiStrLen) + sizeof(m_uiVal_5) + sizeof(m_ucVal_6) + sizeof(m_uiVal_7);
		bytes += static_cast<std::size_t>(m_uiStrLen ? (m_uiStrLen + 1) : 0);
		return bytes;
	}

	auto Arg_Type1::SetStr(const std::string_view msStr) -> void
	{
		m_msStr.assign(msStr);
		m_uiStrLen = static_cast<std::uint32_t>(msStr.size());
	}


	Arg_Type2::Arg_Type2()
	{

	}

	Arg_Type2::Arg_Type2(ZxMem& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type2::Arg_Type2(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto Arg_Type2::Load(ZxMem& vMem) -> void
	{
		vMem >> m_uiParameterType1Count;

		for ([[maybe_unused]] auto idx : std::views::iota(0u, m_uiParameterType1Count))
		{
			m_vcParameterType1.emplace_back(SPT::Arg_Type1{ vMem });
		}
	}

	auto Arg_Type2::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_uiParameterType1Count = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("ArgType1Count").GetStrView());

		for (const auto& type1_json : rfJson.At("ArgType1List").GetArray()) 
		{ 
			m_vcParameterType1.emplace_back(Arg_Type1{ type1_json, nCodePage });
		}
	}

	auto Arg_Type2::Make(ZxMem& amMem) const -> void
	{
		amMem << m_uiParameterType1Count;

		for (const auto& type1: m_vcParameterType1) 
		{
			type1.Make(amMem);
		}
	}

	auto Arg_Type2::Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void
	{
		ZxJson::JArray_t count_arrary;
		count_arrary.reserve(m_vcParameterType1.size());
		for (const auto& type1 : m_vcParameterType1) 
		{
			ZxJson::JValue tmp;
			type1.Make(tmp, nCodePage);
			count_arrary.push_back(std::move(tmp));
		}

		rfJson["ArgType1Count"].Assign(Str::NumToStr("0x%08x", m_uiParameterType1Count));
		rfJson["ArgType1List"].Assign(std::move(count_arrary));
	}

	auto Arg_Type2::SizeBytes() const -> std::size_t
	{
		std::size_t bytes{ sizeof(m_uiParameterType1Count) };

		for (const auto& type1 : m_vcParameterType1) 
		{ 
			bytes += type1.SizeBytes();
		}

		return bytes;
	}


	Arg_Type3::Arg_Type3()
	{

	}

	Arg_Type3::Arg_Type3(ZxMem& vMem)
	{
		this->Load(vMem);
	}

	Arg_Type3::Arg_Type3(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto Arg_Type3::Load(ZxMem& vMem) -> void
	{
		vMem >> m_uiVal_0 >> m_uiVal_1 >> m_uiVal_2;
	}

	auto Arg_Type3::Load(const ZxJson::JValue& rfJson, const std::size_t /*nCodePage*/) -> void
	{
		m_uiVal_0 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_0").GetStrView());
		m_uiVal_1 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_1").GetStrView());
		m_uiVal_2 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_2").GetStrView());
	}

	auto Arg_Type3::Make(ZxMem& amMem) const -> void
	{
		amMem << m_uiVal_0 << m_uiVal_1 << m_uiVal_2;
	}

	auto Arg_Type3::Make(ZxJson::JValue& rfJson, const std::size_t /*nCodePage*/) const -> void
	{
		rfJson["Val_0"] = Str::NumToStr("0x%08x", m_uiVal_0);
		rfJson["Val_1"] = Str::NumToStr("0x%08x", m_uiVal_1);
		rfJson["Val_2"] = Str::NumToStr("0x%08x", m_uiVal_2);
	}

	auto Arg_Type3::SizeBytes() const -> std::size_t
	{
		return sizeof(m_uiVal_0) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2);
	}

}