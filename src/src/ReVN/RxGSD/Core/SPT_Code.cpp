#include "SPT_Code.h"
#include "SPT_Str.h"
#include <ranges>
#include <stdexcept>


namespace ZQF::ReVN::RxGSD::SPT
{
	Code::Code()
	{

	}

	Code::Code(ZxMem& vMem)
	{
		this->Load(vMem);
	}

	Code::Code(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto Code::Load(ZxMem& rfMem) -> void
	{
		rfMem >> m_uiCommand >> m_uiVal_1 >> m_uiVal_2 >> m_uiVal_3 >> m_uiVal_4 >> m_uiSequnece >> m_uiArgType1Count >> m_uiArgType2Count >> m_uiArgType3Count;

		m_uiCommand == 1 ? (void)m_ArgType0.Load(rfMem) : void(0);// Proc Text Struct

		for ([[maybe_unused]] auto idx : std::views::iota(0u, m_uiArgType1Count)) 
		{ 
			m_vcArgType1.emplace_back(rfMem);
		}

		for ([[maybe_unused]]  auto idx : std::views::iota(0u, m_uiArgType2Count))
		{ 
			m_vcArgType2.emplace_back(rfMem);
		}

		for ([[maybe_unused]]  auto idx : std::views::iota(0u, m_uiArgType3Count))
		{ 
			m_vcArgType3.emplace_back(rfMem);
		}
	}

	auto Code::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_uiCommand = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Command").GetStrView());
		m_uiVal_1 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_1").GetStrView());
		m_uiVal_2 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_2").GetStrView());
		m_uiVal_3 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_3").GetStrView());
		m_uiVal_4 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Val_4").GetStrView());
		m_uiSequnece = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Sequnece").GetStrView());
		m_uiArgType1Count = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("ArgType1Count").GetStrView());
		m_uiArgType2Count = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("ArgType2Count").GetStrView());
		m_uiArgType3Count = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("ArgType3Count").GetStrView());

		m_uiCommand == 1 ? (void)m_ArgType0.Load(rfJson.At("ArgType0"), nCodePage) : void(0); // Proc Text Struct

		if (m_uiArgType1Count)
		{
			for (const auto& type1_json : rfJson.At("ArgType1").GetArray())
			{
				m_vcArgType1.emplace_back(SPT::Arg_Type1{ type1_json, nCodePage });
			}
		}

		if (m_uiArgType2Count)
		{
			for (const auto& type2_json : rfJson.At("ArgType2").GetArray())
			{
				m_vcArgType2.emplace_back(SPT::Arg_Type2{ type2_json, nCodePage });
			}
		}

		if (m_uiArgType3Count)
		{
			for (const auto& type3_json : rfJson.At("ArgType3").GetArray())
			{
				m_vcArgType3.emplace_back(SPT::Arg_Type3{ type3_json, nCodePage });
			}
		}
	}

	auto Code::Make(ZxMem& rfMem) const -> void
	{
		rfMem << m_uiCommand << m_uiVal_1 << m_uiVal_2 << m_uiVal_3 << m_uiVal_4 << m_uiSequnece << m_uiArgType1Count << m_uiArgType2Count << m_uiArgType3Count;

		if (m_uiCommand == 1) // Proc Text Struct
		{
			m_ArgType0.Make(rfMem);
		}

		for (const auto& type1 : m_vcArgType1) 
		{ 
			type1.Make(rfMem);
		}

		for (const auto& type2 : m_vcArgType2)
		{ 
			type2.Make(rfMem);
		}

		for (const auto& type3 : m_vcArgType3)
		{ 
			type3.Make(rfMem);
		}
	}

	auto Code::Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void
	{
		rfJson["Command"] = Str::NumToStr("0x%08x", m_uiCommand);
		rfJson["Val_1"] = Str::NumToStr("0x%08x", m_uiVal_1);
		rfJson["Val_2"] = Str::NumToStr("0x%08x", m_uiVal_2);
		rfJson["Val_3"] = Str::NumToStr("0x%08x", m_uiVal_3);
		rfJson["Val_4"] = Str::NumToStr("0x%08x", m_uiVal_4);
		rfJson["Sequnece"] = Str::NumToStr("0x%08x", m_uiSequnece);
		rfJson["ArgType1Count"] = Str::NumToStr("0x%08x", m_uiArgType1Count);
		rfJson["ArgType2Count"] = Str::NumToStr("0x%08x", m_uiArgType2Count);
		rfJson["ArgType3Count"] = Str::NumToStr("0x%08x", m_uiArgType3Count);

		if (m_uiCommand == 1)// Proc Text Struct
		{
			m_ArgType0.Make(rfJson["ArgType0"], nCodePage);
		}

		if (m_vcArgType1.size())
		{
			auto& json_type1 = rfJson["ArgType1"].ToArray();
			json_type1.reserve(m_vcArgType1.size());
			for (const auto& type1 : m_vcArgType1)
			{
				ZxJson::JValue tmp;
				type1.Make(tmp, nCodePage);
				json_type1.push_back(std::move(tmp));
			}
		}

		if (m_vcArgType2.size())
		{
			auto& json_type2 = rfJson["ArgType2"].ToArray();
			json_type2.reserve(m_vcArgType2.size());
			for (auto& type2 : m_vcArgType2)
			{
				ZxJson::JValue tmp;
				type2.Make(tmp, nCodePage);
				json_type2.push_back(std::move(tmp));
			}
		}

		if (m_vcArgType3.size())
		{
			auto& json_type3 = rfJson["ArgType3"].ToArray();
			json_type3.reserve(m_vcArgType3.size());
			for (auto& type3 : m_vcArgType3)
			{
				ZxJson::JValue tmp;
				type3.Make(tmp, nCodePage);
				json_type3.push_back(std::move(tmp));
			}
		}
	}

	auto Code::GetCommand() const -> std::size_t
	{
		return static_cast<std::size_t>(m_uiCommand);
	}

	auto Code::GetArgType0() -> SPT::Arg_Type0&
	{
		return m_ArgType0;
	}

	auto Code::SizeBytes() const -> std::size_t
	{
		std::size_t code_bytes{};

		code_bytes += sizeof(m_uiCommand) + sizeof(m_uiVal_1) + sizeof(m_uiVal_2) + sizeof(m_uiVal_3) + sizeof(m_uiVal_4);
		code_bytes += sizeof(m_uiSequnece) + sizeof(m_uiArgType1Count) + sizeof(m_uiArgType2Count) + sizeof(m_uiArgType3Count);
		code_bytes += m_uiCommand == 1 ? m_ArgType0.SizeBytes() : 0;

		for (auto& type1 : m_vcArgType1)
		{
			code_bytes += type1.SizeBytes();
		}

		for (auto& type2 : m_vcArgType2)
		{
			code_bytes += type2.SizeBytes();
		}

		for (auto& type3 : m_vcArgType3)
		{
			code_bytes += type3.SizeBytes();
		}

		return code_bytes;
	}
}