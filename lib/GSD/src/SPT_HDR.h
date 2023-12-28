#pragma once
#include <string>
#include <vector>

#include "GSD_Types.h"
#include "../../Rut/RxMem.h"
#include "../../Rut/RxJson.h"


namespace GSD::SPT
{
	class Append_Script_Entry
	{
	private:
		uint32_t m_uiStrLen0 = 0;
		std::string m_msStr0;
		uint32_t m_uiStrLen1 = 0;
		std::string m_msStr1;
		uint32_t m_uiUn0 = 0;
		uint32_t m_uiUn1 = 0;
		uint32_t m_uiUn2 = 0;
		char m_aAppend[0x80] = {}; // Maybe there's no data in here.

	public:
		Append_Script_Entry();

		void Load(uint8_t* pData);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};

	class Append_Script
	{
	private:
		uint32_t m_uiScriptCount = 0;
		std::vector<Append_Script_Entry> m_vcInfo;

	public:
		Append_Script();

		void Load(uint8_t* pData);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};

	class Encryptor_Info
	{
	private:
		uint8_t m_ucDecStartIndex = 0;
		uint8_t m_ucDecModeType = 0;
		uint8_t m_ucUn0 = 0;
		uint8_t m_ucUn1 = 0;

	public:
		Encryptor_Info();

		void Load(uint8_t* pData);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};

	class Codes_Info
	{
	private:
		uint32_t m_uiCodeCount = 0;
		uint32_t m_uiUn0 = 0;
		uint32_t m_uiUn1 = 0;
		uint32_t m_uiUn2 = 0;

	public:
		Codes_Info();

		void Load(uint8_t* pInfo);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		size_t GetCodeCount() const;
	};

	class HDR
	{
	private:
		Encryptor_Info m_EncryptorInfo;
		uint32_t m_uiUnCount = 0;
		uint32_t m_uiScriptNameLen = 0;
		std::string m_msScriptName;
		Codes_Info m_CodesInfo;
		Append_Script m_aAppendScript[0xF];
		uint32_t m_uiUnsize = 0;

	public:
		HDR();

		void Load(uint8_t* pData);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		size_t GetCodeCount() const;
	};
}