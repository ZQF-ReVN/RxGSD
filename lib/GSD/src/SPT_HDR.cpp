#include "SPT_HDR.h"
#include "SPT_Str.h"


namespace GSD::SPT
{
	Append_Script_Entry::Append_Script_Entry()
	{

	}

	void Append_Script_Entry::Load(uint8_t* pData)
	{
		uint8_t* cur_ptr = pData;

		m_uiStrLen0 = *(uint32_t*)cur_ptr;
		cur_ptr += 4;

		if (m_uiStrLen0)
		{
			char* str0_ptr = (char*)(cur_ptr);
			m_msStr0 = { str0_ptr , m_uiStrLen0 };
		}
		cur_ptr += m_uiStrLen0;

		m_uiStrLen1 = *(uint32_t*)cur_ptr;
		cur_ptr += 4;

		if (m_uiStrLen1)
		{
			char* str1_ptr = (char*)(cur_ptr);
			m_msStr1 = { str1_ptr , m_uiStrLen1 };
		}
		cur_ptr += m_uiStrLen1;

		m_uiUn0 = *(uint32_t*)cur_ptr;
		cur_ptr += 4;

		m_uiUn1 = *(uint32_t*)cur_ptr;
		cur_ptr += 4;

		m_uiUn2 = *(uint32_t*)cur_ptr;
		cur_ptr += 4;

		memcpy(m_aAppend, cur_ptr, sizeof(m_aAppend));

	}

	Rut::RxMem::Auto Append_Script_Entry::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		*(uint32_t*)cur_ptr = m_uiStrLen0;
		cur_ptr += 4;

		if (m_uiStrLen0)
		{
			memcpy(cur_ptr, m_msStr0.data(), m_uiStrLen0);
		}
		cur_ptr += m_uiStrLen0;

		*(uint32_t*)cur_ptr = m_uiStrLen1;
		cur_ptr += 4;

		if (m_uiStrLen1)
		{
			memcpy(cur_ptr, m_msStr1.data(), m_uiStrLen1);
		}
		cur_ptr += m_uiStrLen1;

		*(uint32_t*)cur_ptr = m_uiUn0;
		cur_ptr += 4;
		*(uint32_t*)cur_ptr = m_uiUn1;
		cur_ptr += 4;
		*(uint32_t*)cur_ptr = m_uiUn2;
		cur_ptr += 4;

		memcpy(cur_ptr, m_aAppend, sizeof(m_aAppend));

		return mem_data;
	}

	Rut::RxJson::JValue Append_Script_Entry::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"StrLen0"] = Str::NumToStr(L"0x%08x", m_uiStrLen0);
		json[L"Str0"] = Str::LoadANSI(m_msStr0, nCodePage);
		json[L"StrLen1"] = Str::NumToStr(L"0x%08x", m_uiStrLen1);
		json[L"Str1"] = Str::LoadANSI(m_msStr1, nCodePage);
		json[L"Un0"] = Str::NumToStr(L"0x%08x", m_uiUn0);
		json[L"Un1"] = Str::NumToStr(L"0x%08x", m_uiUn1);
		json[L"Un2"] = Str::NumToStr(L"0x%08x", m_uiUn2);

		Rut::RxJson::JValue& json_append = json[L"AppendData"];
		uint32_t append_size = (sizeof(m_aAppend) / 4);
		uint32_t* append_ptr = (uint32_t*)m_aAppend;
		for (size_t ite_append = 0; ite_append < append_size; ite_append++)
		{
			json_append.Append(Str::NumToStr(L"0x%08x", append_ptr[ite_append]));
		}
		return json;
	}

	size_t Append_Script_Entry::GetSize() const
	{
		return sizeof(m_uiStrLen0) + m_msStr0.size() + sizeof(m_uiStrLen1) + m_msStr1.size() + sizeof(m_uiUn0) + sizeof(m_uiUn1) + sizeof(m_uiUn2) + sizeof(m_aAppend);
	}


	Append_Script::Append_Script()
	{

	}

	void Append_Script::Load(uint8_t* pData)
	{
		uint8_t* cur_prt = pData;

		m_uiScriptCount = *(uint32_t*)pData;
		cur_prt += 4;

		for (size_t ite_info = 0; ite_info < m_uiScriptCount; ite_info++)
		{
			Append_Script_Entry info;
			info.Load(cur_prt);
			cur_prt += info.GetSize();
			m_vcInfo.emplace_back(std::move(info));
		}
	}

	Rut::RxMem::Auto Append_Script::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		*(uint32_t*)cur_ptr = m_uiScriptCount;
		cur_ptr += 4;

		for (const auto& info : m_vcInfo)
		{
			Rut::RxMem::Auto data = info.Make();
			memcpy(cur_ptr, data.GetPtr(), data.GetSize());
			cur_ptr += data.GetSize();
		}

		return mem_data;
	}

	Rut::RxJson::JValue Append_Script::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		json[L"ScriptCount"] = Str::NumToStr(L"0x%08x", m_uiScriptCount);

		Rut::RxJson::JValue& json_info = json[L"InfoList"];
		for (const auto& info : m_vcInfo)
		{
			json_info.Append(info.Make(nCodePage));
		}

		return json;
	}

	size_t Append_Script::GetSize() const
	{
		size_t size = 0;

		size += sizeof(m_uiScriptCount);

		for (auto& info : m_vcInfo)
		{
			size += info.GetSize();
		}

		return size;
	}


	Encryptor_Info::Encryptor_Info()
	{

	}

	void Encryptor_Info::Load(uint8_t* pData)
	{
		m_ucDecStartIndex = pData[0];
		m_ucDecModeType = pData[1];
		m_ucUn0 = pData[2];
		m_ucUn1 = pData[3];
	}

	Rut::RxMem::Auto Encryptor_Info::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		cur_ptr[0] = m_ucDecStartIndex;
		cur_ptr[1] = m_ucDecModeType;
		cur_ptr[2] = m_ucUn0;
		cur_ptr[3] = m_ucUn1;

		return mem_data;
	}

	Rut::RxJson::JValue Encryptor_Info::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		json[L"DecStartIndex"] = Str::NumToStr(L"0x%02x", m_ucDecStartIndex);
		json[L"DecModeType"] = Str::NumToStr(L"0x%02x", m_ucDecModeType);
		json[L"Un0"] = Str::NumToStr(L"0x%02x", m_ucUn0);
		json[L"Un1"] = Str::NumToStr(L"0x%02x", m_ucUn1);

		return json;
	}

	size_t Encryptor_Info::GetSize() const
	{
		return sizeof(m_ucDecStartIndex) + sizeof(m_ucDecModeType) + sizeof(m_ucUn0) + sizeof(m_ucUn1);
	}


	Codes_Info::Codes_Info()
	{

	}

	void Codes_Info::Load(uint8_t* pInfo)
	{
		uint32_t* info_chunk_ptr = (uint32_t*)pInfo;

		m_uiCodeCount = info_chunk_ptr[0];
		m_uiUn0 = info_chunk_ptr[1];
		m_uiUn1 = info_chunk_ptr[2];
		m_uiUn2 = info_chunk_ptr[3];
	}

	Rut::RxMem::Auto Codes_Info::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		uint32_t* tmp_ptr = (uint32_t*)cur_ptr;
		tmp_ptr[0] = m_uiCodeCount;
		tmp_ptr[1] = m_uiUn0;
		tmp_ptr[2] = m_uiUn1;
		tmp_ptr[3] = m_uiUn2;

		return mem_data;
	}

	Rut::RxJson::JValue Codes_Info::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		json[L"CodeCount"] = Str::NumToStr(L"0x%08x", m_uiCodeCount);
		json[L"Un0"] = Str::NumToStr(L"0x%08x", m_uiUn0);
		json[L"Un1"] = Str::NumToStr(L"0x%08x", m_uiUn1);
		json[L"Un2"] = Str::NumToStr(L"0x%08x", m_uiUn2);

		return json;
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

	void HDR::Load(uint8_t* pData)
	{
		uint8_t* cur_ptr = pData;

		m_EncryptorInfo.Load(pData);
		cur_ptr += m_EncryptorInfo.GetSize();

		m_uiUnCount = *((uint32_t*)(cur_ptr));
		if (m_uiUnCount != 0) { throw std::runtime_error("SPT_HDR: Unknow Format"); }
		cur_ptr += 4;

		m_uiScriptNameLen = *(uint32_t*)(cur_ptr);
		cur_ptr += 4;

		char* script_name_ptr = (char*)(cur_ptr);
		m_msScriptName = { script_name_ptr, m_uiScriptNameLen };
		cur_ptr += m_uiScriptNameLen;

		m_CodesInfo.Load(cur_ptr);
		cur_ptr += m_CodesInfo.GetSize();

		for (auto& append_script : m_aAppendScript)
		{
			append_script.Load(cur_ptr);
			cur_ptr += append_script.GetSize();
		}

		m_uiUnsize = *(uint32_t*)cur_ptr;
	}

	Rut::RxMem::Auto HDR::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		Rut::RxMem::Auto enc_info_mem = m_EncryptorInfo.Make();
		memcpy(cur_ptr, enc_info_mem.GetPtr(), enc_info_mem.GetSize());
		cur_ptr += enc_info_mem.GetSize();

		*((uint32_t*)(cur_ptr)) = m_uiUnCount;
		cur_ptr += 4;
		*(uint32_t*)(cur_ptr) = m_uiScriptNameLen;
		cur_ptr += 4;

		memcpy(cur_ptr, m_msScriptName.data(), m_uiScriptNameLen);
		cur_ptr += m_uiScriptNameLen;

		Rut::RxMem::Auto codes_info_mem = m_CodesInfo.Make();
		memcpy(cur_ptr, codes_info_mem.GetPtr(), codes_info_mem.GetSize());
		cur_ptr += codes_info_mem.GetSize();

		for (auto& append_script : m_aAppendScript)
		{
			Rut::RxMem::Auto append_script_mem = append_script.Make();
			memcpy(cur_ptr, append_script_mem.GetPtr(), append_script_mem.GetSize());
			cur_ptr += append_script_mem.GetSize();
		}

		*(uint32_t*)cur_ptr = m_uiUnsize;

		return mem_data;
	}

	Rut::RxJson::JValue HDR::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		json[L"EncryptorInfo"] = m_EncryptorInfo.Make(nCodePage);
		json[L"UnCount"] = Str::NumToStr(L"0x%08x", m_uiUnCount);
		json[L"ScriptNameLen"] = Str::NumToStr(L"0x%08x", m_uiScriptNameLen);
		json[L"ScriptName"] = Str::LoadANSI(m_msScriptName, 932);
		json[L"CodesInfo"] = m_CodesInfo.Make(nCodePage);

		Rut::RxJson::JValue& json_append_script = json[L"Append"];
		for (const auto& append_script : m_aAppendScript)
		{
			json_append_script.Append(append_script.Make(nCodePage));
		}

		json[L"Unsize"] = Str::NumToStr(L"0x%08x", m_uiUnsize);

		return json;
	}

	size_t HDR::GetSize() const
	{
		size_t size = 0;
		{
			size += m_EncryptorInfo.GetSize() + sizeof(m_uiUnCount) + sizeof(m_uiScriptNameLen) + m_msScriptName.size() + m_CodesInfo.GetSize();
			for (auto& append_script : m_aAppendScript)
			{
				size += append_script.GetSize();
			}
			size += sizeof(m_uiUnsize);
		}
		return size;
	}

	size_t HDR::GetCodeCount() const
	{
		return m_CodesInfo.GetCodeCount();
	}
}
