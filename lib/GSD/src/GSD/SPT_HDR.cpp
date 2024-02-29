#include "SPT_HDR.h"
#include "SPT_Str.h"

#include <ranges>


namespace GSD::SPT
{
	Append_Script_Entry::Append_Script_Entry()
	{

	}

	Append_Script_Entry::Append_Script_Entry(Rut::RxMem::View& vMem)
	{
		this->Load(vMem);
	}

	Append_Script_Entry::Append_Script_Entry(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	void Append_Script_Entry::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiStrLen0;

		if (m_uiStrLen0)
		{
			m_msStr0 = { vMem.CurPtr<char>() , m_uiStrLen0 };
			vMem.Skip(m_uiStrLen0);
		}

		vMem >> m_uiStrLen1;

		if (m_uiStrLen1)
		{
			m_msStr1 = { vMem.CurPtr<char>() , m_uiStrLen1 };
			vMem.Skip(m_uiStrLen1);
		}
		
		vMem >> m_uiUn0 >> m_uiUn1 >> m_uiUn2 >> m_aAppend;
	}

	void Append_Script_Entry::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiStrLen0 = Str::StrToNum(L"0x%08x", rfJson[L"StrLen0"]);
		m_msStr0 = Str::MakeANSI(rfJson[L"Str0"], nCodePage);
		m_uiStrLen1 = Str::StrToNum(L"0x%08x", rfJson[L"StrLen1"]);
		m_msStr1 = Str::MakeANSI(rfJson[L"Str1"], nCodePage);
		m_uiUn0 = Str::StrToNum(L"0x%08x", rfJson[L"Un0"]);
		m_uiUn1 = Str::StrToNum(L"0x%08x", rfJson[L"Un1"]);
		m_uiUn2 = Str::StrToNum(L"0x%08x", rfJson[L"Un2"]);

		Rut::RxMem::View append_view = m_aAppend;
		for (const auto& append : rfJson[L"AppendData"].ToAry())
		{
			append_view << (uint32_t)Str::StrToNum(L"0x%08x", append);
		}
	}

	void Append_Script_Entry::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());

		Rut::RxMem::View view = amMem.GetView();

		view << m_uiStrLen0;
		if (m_uiStrLen0)
		{
			view.Write(m_msStr0.data(), m_uiStrLen0);
		}

		view << m_uiStrLen1;
		if (m_uiStrLen1)
		{
			view.Write(m_msStr1.data(), m_uiStrLen1);
		}

		view << m_uiUn0 << m_uiUn1 << m_uiUn2 << m_aAppend;
	}

	void Append_Script_Entry::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"StrLen0"] = Str::NumToStr(L"0x%08x", m_uiStrLen0);
		rfJson[L"Str0"] = Str::LoadANSI(m_msStr0, nCodePage);
		rfJson[L"StrLen1"] = Str::NumToStr(L"0x%08x", m_uiStrLen1);
		rfJson[L"Str1"] = Str::LoadANSI(m_msStr1, nCodePage);
		rfJson[L"Un0"] = Str::NumToStr(L"0x%08x", m_uiUn0);
		rfJson[L"Un1"] = Str::NumToStr(L"0x%08x", m_uiUn1);
		rfJson[L"Un2"] = Str::NumToStr(L"0x%08x", m_uiUn2);

		Rut::RxJson::JArray& json_append = rfJson[L"AppendData"].ToAry();
		std::span<uint32_t> append_sp{ (uint32_t*)m_aAppend, sizeof(m_aAppend) / 4 };
		json_append.reserve(append_sp.size());
		for (auto& ite_append : append_sp)
		{
			json_append.emplace_back(Str::NumToStr(L"0x%08x", ite_append));
		}
	}

	size_t Append_Script_Entry::GetSize() const
	{
		return sizeof(m_uiStrLen0) + m_msStr0.size() + sizeof(m_uiStrLen1) + m_msStr1.size() + sizeof(m_uiUn0) + sizeof(m_uiUn1) + sizeof(m_uiUn2) + sizeof(m_aAppend);
	}


	Append_Script::Append_Script()
	{

	}

	void Append_Script::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiScriptCount;

		for (auto i : std::views::iota(0u, m_uiScriptCount)) 
		{
			m_vcInfo.emplace_back(vMem);
		}
	}

	void Append_Script::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiScriptCount = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"ScriptCount"]);

		for (auto& entry_json : rfJson[L"InfoList"].ToAry()) 
		{ 
			m_vcInfo.emplace_back(entry_json, nCodePage);
		}
	}

	void Append_Script::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());

		Rut::RxMem::View view = amMem.GetView();

		view << m_uiScriptCount;

		Rut::RxMem::Auto buffer;
		for (const auto& info : m_vcInfo) 
		{ 
			info.Make(buffer);
			view << buffer;
		}
	}

	void Append_Script::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"ScriptCount"] = Str::NumToStr(L"0x%08x", m_uiScriptCount);

		Rut::RxJson::JArray& json_info = rfJson[L"InfoList"].ToAry();
		json_info.reserve(m_vcInfo.size());
		for (const auto& info : m_vcInfo) 
		{
			Rut::RxJson::JValue tmp;
			info.Make(tmp, nCodePage);
			json_info.emplace_back(std::move(tmp));
		}
	}

	size_t Append_Script::GetSize() const
	{
		size_t size = sizeof(m_uiScriptCount);

		for (auto& info : m_vcInfo) 
		{ 
			size += info.GetSize();
		}

		return size;
	}


	Encryptor_Info::Encryptor_Info()
	{

	}

	void Encryptor_Info::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_ucDecStartIndex >> m_ucDecModeType >> m_ucUn0 >> m_ucUn1;
	}

	void Encryptor_Info::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_ucDecStartIndex = (uint8_t)Str::StrToNum(L"0x%02x", rfJson[L"DecStartIndex"]);
		m_ucDecModeType = (uint8_t)Str::StrToNum(L"0x%02x", rfJson[L"DecModeType"]);
		m_ucUn0 = (uint8_t)Str::StrToNum(L"0x%02x", rfJson[L"Un0"]);
		m_ucUn1 = (uint8_t)Str::StrToNum(L"0x%02x", rfJson[L"Un1"]);
	}

	void Encryptor_Info::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());
		amMem.GetView() << m_ucDecStartIndex << m_ucDecModeType << m_ucUn0 << m_ucUn1;
	}

	void Encryptor_Info::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"DecStartIndex"] = Str::NumToStr(L"0x%02x", m_ucDecStartIndex);
		rfJson[L"DecModeType"] = Str::NumToStr(L"0x%02x", m_ucDecModeType);
		rfJson[L"Un0"] = Str::NumToStr(L"0x%02x", m_ucUn0);
		rfJson[L"Un1"] = Str::NumToStr(L"0x%02x", m_ucUn1);
	}

	size_t Encryptor_Info::GetSize() const
	{
		return sizeof(m_ucDecStartIndex) + sizeof(m_ucDecModeType) + sizeof(m_ucUn0) + sizeof(m_ucUn1);
	}


	Codes_Info::Codes_Info()
	{

	}

	void Codes_Info::Load(Rut::RxMem::View& vMem)
	{
		vMem >> m_uiCodeCount >> m_uiUn0 >> m_uiUn1 >> m_uiUn2;
	}

	void Codes_Info::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_uiCodeCount = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"CodeCount"]);
		m_uiUn0 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Un0"]);
		m_uiUn1 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Un1"]);
		m_uiUn2 = (uint32_t)Str::StrToNum(L"0x%08x", rfJson[L"Un2"]);
	}

	void Codes_Info::Make(Rut::RxMem::Auto& amMem) const
	{
		amMem.SetSize(this->GetSize());
		amMem.GetView() << m_uiCodeCount << m_uiUn0 << m_uiUn1 << m_uiUn2;
	}

	void Codes_Info::Make(Rut::RxJson::JValue& rfJson, size_t nCodePage) const
	{
		rfJson[L"CodeCount"] = Str::NumToStr(L"0x%08x", m_uiCodeCount);
		rfJson[L"Un0"] = Str::NumToStr(L"0x%08x", m_uiUn0);
		rfJson[L"Un1"] = Str::NumToStr(L"0x%08x", m_uiUn1);
		rfJson[L"Un2"] = Str::NumToStr(L"0x%08x", m_uiUn2);
	}

	size_t Codes_Info::GetSize() const
	{
		return sizeof(m_uiCodeCount) + sizeof(m_uiUn0) + sizeof(m_uiUn1) + sizeof(m_uiUn2);;
	}

	size_t Codes_Info::GetCodeCount() const
	{
		return m_uiCodeCount;
	}


	HDR::HDR()
	{

	}

	void HDR::Load(Rut::RxMem::View& vMem)
	{
		m_EncryptorInfo.Load(vMem);

		vMem >> m_uiUnCount;

		if (m_uiUnCount != 0) { throw std::runtime_error("SPT_HDR: Unknow Format"); }

		vMem >> m_uiScriptNameLen;

		if (m_uiScriptNameLen)
		{
			m_msScriptName = { vMem.CurPtr<char>(), m_uiScriptNameLen };
			vMem.Skip(m_uiScriptNameLen);
		}

		m_CodesInfo.Load(vMem);

		for (auto& append_script : m_aAppendScript) 
		{ 
			append_script.Load(vMem);
		}

		vMem >> m_uiUnsize;
	}

	void HDR::Load(const std::filesystem::path& phSpt)
	{
		Rut::RxMem::Auto spt(phSpt);
		Rut::RxMem::View view = spt.GetView();
		this->Load(view);
	}

	void HDR::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_EncryptorInfo.Load(rfJson[L"EncryptorInfo"], nCodePage);
		m_uiUnCount = Str::StrToNum(L"0x%08x", rfJson[L"UnCount"]);
		m_uiScriptNameLen = Str::StrToNum(L"0x%08x", rfJson[L"ScriptNameLen"]);
		m_msScriptName = Str::MakeANSI(rfJson[L"ScriptName"], nCodePage);
		m_CodesInfo.Load(rfJson[L"CodesInfo"], nCodePage);

		for (auto [append, append_json] : std::views::zip(m_aAppendScript, rfJson[L"Append"].ToAry()))
		{
			append.Load(append_json, nCodePage);
		}

		m_uiUnsize = Str::StrToNum(L"0x%08x", rfJson[L"Unsize"]);
	}

	Rut::RxMem::Auto HDR::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		Rut::RxMem::View view = mem_data.GetView();

		Rut::RxMem::Auto buffer;

		m_EncryptorInfo.Make(buffer);
		view << buffer;

		view << m_uiUnCount << m_uiScriptNameLen;

		if (m_uiScriptNameLen)
		{
			view.Write(m_msScriptName.data(), m_uiScriptNameLen);
		}

		m_CodesInfo.Make(buffer);
		view << buffer;

		for (auto& append_script : m_aAppendScript)
		{
			append_script.Make(buffer);
			view << buffer;
		}

		view << m_uiUnsize;

		return mem_data;
	}

	Rut::RxJson::JValue HDR::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		m_EncryptorInfo.Make(json[L"EncryptorInfo"], nCodePage);
		json[L"UnCount"] = Str::NumToStr(L"0x%08x", m_uiUnCount);
		json[L"ScriptNameLen"] = Str::NumToStr(L"0x%08x", m_uiScriptNameLen);
		json[L"ScriptName"] = Str::LoadANSI(m_msScriptName, nCodePage);
		m_CodesInfo.Make(json[L"CodesInfo"], nCodePage);

		Rut::RxJson::JArray& json_append_script = json[L"Append"].ToAry();
		json_append_script.reserve(sizeof(m_aAppendScript) / sizeof(Append_Script));
		for (const auto& append_script : m_aAppendScript)
		{
			Rut::RxJson::JValue tmp;
			append_script.Make(tmp, nCodePage);
			json_append_script.emplace_back(std::move(tmp));
		}

		json[L"Unsize"] = Str::NumToStr(L"0x%08x", m_uiUnsize);

		return json;
	}

	size_t HDR::GetSize() const
	{
		size_t size = m_EncryptorInfo.GetSize() + sizeof(m_uiUnCount) + sizeof(m_uiScriptNameLen) + m_msScriptName.size() + m_CodesInfo.GetSize();

		for (auto& append_script : m_aAppendScript)
		{
			size += append_script.GetSize();
		}

		size += sizeof(m_uiUnsize);

		return size;
	}

	size_t HDR::GetCodeCount() const
	{
		return m_CodesInfo.GetCodeCount();
	}
}
