#include "SPT_HDR.h"
#include "SPT_Str.h"
#include <ranges>


namespace ZQF::ReVN::RxGSD::SPT
{
	Append_Script_Entry::Append_Script_Entry()
	{

	}

	Append_Script_Entry::Append_Script_Entry(ZxMem& vMem)
	{
		this->Load(vMem);
	}

	Append_Script_Entry::Append_Script_Entry(const ZxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto Append_Script_Entry::Load(ZxMem& vMem) -> void
	{
		vMem >> m_uiStrLen0;

		if (m_uiStrLen0)
		{
			m_msStr0.assign(vMem.PtrCur<char*>() , m_uiStrLen0);
			vMem.PosInc(m_uiStrLen0);
		}

		vMem >> m_uiStrLen1;

		if (m_uiStrLen1)
		{
			m_msStr1.assign(vMem.PtrCur<char*>(), m_uiStrLen1);
			vMem.PosInc(m_uiStrLen1);
		}
		
		vMem >> m_uiUn0 >> m_uiUn1 >> m_uiUn2 >> m_aAppend;
	}

	auto Append_Script_Entry::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_uiStrLen0 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("StrLen0").GetStrView());
		m_msStr0 = Str::MakeANSI(rfJson.At("Str0").GetStrView(), nCodePage);
		m_uiStrLen1 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("StrLen1").GetStrView());
		m_msStr1 = Str::MakeANSI(rfJson.At("Str1").GetStrView(), nCodePage);
		m_uiUn0 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un0").GetStrView());
		m_uiUn1 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un1").GetStrView());
		m_uiUn2 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un2").GetStrView());

		ZxMem append_view{ sizeof(m_aAppend) };
		for (const auto& append : rfJson.At("AppendData").GetArray())
		{
			append_view << Str::StrToNum<std::uint32_t>("0x%08x", append.GetStrView());
		}

		append_view.PosRewind() >> m_aAppend;
	}

	auto Append_Script_Entry::Make(ZxMem& amMem) const -> void
	{
		amMem << m_uiStrLen0;
		if (m_uiStrLen0)
		{
			amMem.Write(std::span{ m_msStr0.data(), m_uiStrLen0 });
		}

		amMem << m_uiStrLen1;
		if (m_uiStrLen1)
		{
			amMem.Write(std::span{ m_msStr1.data(), m_uiStrLen1 });
		}

		amMem << m_uiUn0 << m_uiUn1 << m_uiUn2 << m_aAppend;
	}

	auto Append_Script_Entry::Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void
	{
		rfJson["StrLen0"] = Str::NumToStr("0x%08x", m_uiStrLen0);
		rfJson["Str0"] = Str::LoadANSI(m_msStr0, nCodePage);
		rfJson["StrLen1"] = Str::NumToStr("0x%08x", m_uiStrLen1);
		rfJson["Str1"] = Str::LoadANSI(m_msStr1, nCodePage);
		rfJson["Un0"] = Str::NumToStr("0x%08x", m_uiUn0);
		rfJson["Un1"] = Str::NumToStr("0x%08x", m_uiUn1);
		rfJson["Un2"] = Str::NumToStr("0x%08x", m_uiUn2);

		auto& json_append = rfJson["AppendData"].ToArray();
		std::span<uint32_t> append_sp{ (uint32_t*)m_aAppend, sizeof(m_aAppend) / 4 };
		json_append.reserve(append_sp.size());
		for (auto& ite_append : append_sp)
		{
			json_append.emplace_back(Str::NumToStr("0x%08x", ite_append));
		}
	}

	auto Append_Script_Entry::SizeBytes() const -> std::size_t
	{
		return sizeof(m_uiStrLen0) + m_msStr0.size() + sizeof(m_uiStrLen1) + m_msStr1.size() + sizeof(m_uiUn0) + sizeof(m_uiUn1) + sizeof(m_uiUn2) + sizeof(m_aAppend);
	}


	Append_Script::Append_Script()
	{

	}

	void Append_Script::Load(ZxMem& vMem)
	{
		vMem >> m_uiScriptCount;

		for ([[maybe_unused]] auto idx : std::views::iota(0u, m_uiScriptCount)) 
		{
			m_vcInfo.emplace_back(Append_Script_Entry{ vMem });
		}
	}

	void Append_Script::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		m_uiScriptCount = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("ScriptCount").GetStrView());

		for (const auto& entry_json : rfJson.At("InfoList").GetArray()) 
		{ 
			m_vcInfo.emplace_back(SPT::Append_Script_Entry{ entry_json, nCodePage });
		}
	}

	void Append_Script::Make(ZxMem& amMem) const
	{
		amMem << m_uiScriptCount;

		for (const auto& info : m_vcInfo) 
		{ 
			info.Make(amMem);
		}
	}

	void Append_Script::Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const
	{
		rfJson["ScriptCount"] = Str::NumToStr("0x%08x", m_uiScriptCount);

		auto& json_info = rfJson["InfoList"].ToArray();
		json_info.reserve(m_vcInfo.size());
		for (const auto& info : m_vcInfo) 
		{
			ZxJson::JValue tmp;
			info.Make(tmp, nCodePage);
			json_info.emplace_back(std::move(tmp));
		}
	}

	auto Append_Script::SizeBytes() const -> std::size_t
	{
		std::size_t bytes = sizeof(m_uiScriptCount);

		for (auto& info : m_vcInfo) 
		{ 
			bytes += info.SizeBytes();
		}

		return bytes;
	}


	Encryptor_Info::Encryptor_Info()
	{

	}

	auto Encryptor_Info::Load(ZxMem& vMem) -> void
	{
		vMem >> m_ucDecStartIndex >> m_ucDecModeType >> m_ucUn0 >> m_ucUn1;
	}

	auto Encryptor_Info::Load(const ZxJson::JValue& rfJson, const std::size_t /*nCodePage*/) -> void
	{
		m_ucDecStartIndex = Str::StrToNum<std::uint8_t>("0x%02x", rfJson.At("DecStartIndex").GetStrView());
		m_ucDecModeType = Str::StrToNum<std::uint8_t>("0x%02x", rfJson.At("DecModeType").GetStrView());
		m_ucUn0 = Str::StrToNum<std::uint8_t>("0x%02x", rfJson.At("Un0").GetStrView());
		m_ucUn1 = Str::StrToNum<std::uint8_t>("0x%02x", rfJson.At("Un1").GetStrView());
	}

	auto Encryptor_Info::Make(ZxMem& amMem) const -> void
	{
		amMem << m_ucDecStartIndex << m_ucDecModeType << m_ucUn0 << m_ucUn1;
	}

	auto Encryptor_Info::Make(ZxJson::JValue& rfJson, const std::size_t /*nCodePage*/) const -> void
	{
		rfJson["DecStartIndex"] = Str::NumToStr("0x%02x", m_ucDecStartIndex);
		rfJson["DecModeType"] = Str::NumToStr("0x%02x", m_ucDecModeType);
		rfJson["Un0"] = Str::NumToStr("0x%02x", m_ucUn0);
		rfJson["Un1"] = Str::NumToStr("0x%02x", m_ucUn1);
	}

	auto Encryptor_Info::SizeBytes() const -> std::size_t
	{
		return sizeof(m_ucDecStartIndex) + sizeof(m_ucDecModeType) + sizeof(m_ucUn0) + sizeof(m_ucUn1);
	}


	Codes_Info::Codes_Info()
	{

	}

	auto Codes_Info::Load(ZxMem& vMem) -> void
	{
		vMem >> m_uiCodeCount >> m_uiUn0 >> m_uiUn1 >> m_uiUn2;
	}

	auto Codes_Info::Load(const ZxJson::JValue& rfJson, const std::size_t /*nCodePage*/) -> void
	{
		m_uiCodeCount = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("CodeCount").GetStrView());
		m_uiUn0 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un0").GetStrView());
		m_uiUn1 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un1").GetStrView());
		m_uiUn2 = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Un2").GetStrView());
	}

	auto Codes_Info::Make(ZxMem& amMem) const -> void
	{
		amMem << m_uiCodeCount << m_uiUn0 << m_uiUn1 << m_uiUn2;
	}

	auto Codes_Info::Make(ZxJson::JValue& rfJson, const std::size_t /*nCodePage*/) const -> void
	{
		rfJson["CodeCount"] = Str::NumToStr("0x%08x", m_uiCodeCount);
		rfJson["Un0"] = Str::NumToStr("0x%08x", m_uiUn0);
		rfJson["Un1"] = Str::NumToStr("0x%08x", m_uiUn1);
		rfJson["Un2"] = Str::NumToStr("0x%08x", m_uiUn2);
	}

	auto Codes_Info::GetCodeCount() const -> std::size_t
	{
		return m_uiCodeCount;
	}

	auto Codes_Info::SizeBytes() const -> std::size_t
	{
		return sizeof(m_uiCodeCount) + sizeof(m_uiUn0) + sizeof(m_uiUn1) + sizeof(m_uiUn2);;
	}


	HDR::HDR()
	{

	}

	void HDR::Load(ZxMem& vMem)
	{
		m_EncryptorInfo.Load(vMem);

		vMem >> m_uiUnCount;

		if (m_uiUnCount != 0) { throw std::runtime_error("SPT_HDR: Unknow Format"); }

		vMem >> m_uiScriptNameLen;

		if (m_uiScriptNameLen)
		{
			m_msScriptName = { vMem.PtrCur<char*>(), m_uiScriptNameLen };
			vMem.PosInc(m_uiScriptNameLen);
		}

		m_CodesInfo.Load(vMem);

		for (auto& append_script : m_aAppendScript) 
		{ 
			append_script.Load(vMem);
		}

		vMem >> m_uiUnsize;
	}

	auto HDR::Load(const std::string_view msSpt) -> void
	{
		ZxMem spt{ msSpt };
		this->Load(spt);
	}

	auto HDR::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_EncryptorInfo.Load(rfJson.At("EncryptorInfo"), nCodePage);
		m_uiUnCount = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("UnCount").GetStrView());
		m_uiScriptNameLen = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("ScriptNameLen").GetStrView());
		m_msScriptName = Str::MakeANSI(rfJson.At("ScriptName").GetStrView(), nCodePage);
		m_CodesInfo.Load(rfJson.At("CodesInfo"), nCodePage);

		for (auto [append, append_json] : std::views::zip(m_aAppendScript, rfJson.At("Append").GetArray()))
		{
			append.Load(append_json, nCodePage);
		}

		m_uiUnsize = Str::StrToNum<std::uint32_t>("0x%08x", rfJson.At("Unsize").GetStrView());
	}

	auto HDR::Make() const -> ZxMem
	{
		ZxMem mem_data{ this->SizeBytes() };

		m_EncryptorInfo.Make(mem_data);

		mem_data << m_uiUnCount << m_uiScriptNameLen;

		if (m_uiScriptNameLen)
		{
			mem_data.Write(std::span{ m_msScriptName.data(), m_uiScriptNameLen });
		}

		m_CodesInfo.Make(mem_data);

		for (const auto& append_script : m_aAppendScript)
		{
			append_script.Make(mem_data);
		}

		mem_data << m_uiUnsize;

		return mem_data.PosRewind();
	}

	auto HDR::Make(const std::size_t nCodePage) const -> ZxJson::JValue
	{
		ZxJson::JValue json;

		m_EncryptorInfo.Make(json["EncryptorInfo"], nCodePage);
		json["UnCount"] = Str::NumToStr("0x%08x", m_uiUnCount);
		json["ScriptNameLen"] = Str::NumToStr("0x%08x", m_uiScriptNameLen);
		json["ScriptName"] = Str::LoadANSI(m_msScriptName, nCodePage);
		m_CodesInfo.Make(json["CodesInfo"], nCodePage);

		auto& json_append_script = json["Append"].ToArray();
		json_append_script.reserve(sizeof(m_aAppendScript) / sizeof(Append_Script));
		for (const auto& append_script : m_aAppendScript)
		{
			ZxJson::JValue tmp;
			append_script.Make(tmp, nCodePage);
			json_append_script.emplace_back(std::move(tmp));
		}

		json["Unsize"] = Str::NumToStr("0x%08x", m_uiUnsize);

		return json;
	}

	auto HDR::GetCodeCount() const -> std::size_t
	{
		return m_CodesInfo.GetCodeCount();
	}

	auto HDR::SizeBytes() const -> std::size_t
	{
		std::size_t bytes{ m_EncryptorInfo.SizeBytes() + sizeof(m_uiUnCount) + sizeof(m_uiScriptNameLen) + m_msScriptName.size() + m_CodesInfo.SizeBytes() };

		for (const auto& append_script : m_aAppendScript)
		{
			bytes += append_script.SizeBytes();
		}

		bytes += sizeof(m_uiUnsize);

		return bytes;
	}
}
