#pragma once
#include <string>
#include <vector>
#include <stdexcept>

#include "../../Rut/RxMem.h"
#include "../../Rut/RxJson.h"
#include "GSD_Types.h"


namespace GSD::SPT
{
	template <typename SPT_OBJ_T>
	void CheckDumpDataMatched(uint8_t* pOrg, SPT_OBJ_T& rfOBj)
	{
		Rut::RxMem::Auto mem_data = rfOBj.Dump();
		if (memcmp(pOrg, mem_data.GetPtr(), mem_data.GetSize()))
		{
			throw std::runtime_error("Dump data mismatch");
		}
	}


	class Code_Parameter_Type0
	{
	private:
		uint32_t m_uiUn0 = 0;
		uint32_t m_uiUn1 = 0;
		uint32_t m_uiUn2 = 0;
		uint32_t m_uiUn3 = 0;
		uint32_t m_uiStrType0Len = 0; // dbcs char count
		uint32_t m_uiStrType1Len = 0;
		uint32_t m_uiStrType2Len = 0;

		std::vector<SPT_Char_Entry> m_vcCharList;
		std::string m_msStrType1;
		std::string m_msStrType2;

	private:
		static bool IsDBCS(uint32_t uiChar);
		static void DBCSReadChar(std::string& msText, uint32_t uiChar);

		static std::string ParseSptText(const std::vector<SPT_Char_Entry>& rfCharEntries);
		static std::vector<SPT_Char_Entry> MakeSptText(const std::string& msText);

	public:
		Code_Parameter_Type0();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		void SetType0Text(const std::string& msText);
		size_t GetType0TextLen()const;
		std::string GetType0Text() const;
		size_t GetSize() const;
	};

	class Code_Parameter_Type1
	{
	private:
		uint32_t m_uiVal_0 = 0;
		uint32_t m_uiVal_1 = 0;
		uint32_t m_uiVal_2 = 0;
		uint32_t m_uiVal_3 = 0;
		uint32_t m_uiStrLen = 0;
		uint32_t m_uiVal_5 = 0;
		uint8_t m_uiVal_6 = 0;
		uint32_t m_uiVal_7 = 0;
		std::string m_msStr;

	public:
		Code_Parameter_Type1();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};

	class Code_Parameter_Type2
	{
	private:
		uint32_t m_uiParameterType1Count = 0;
		std::vector<Code_Parameter_Type1> m_vcParameterType1;

	public:
		Code_Parameter_Type2();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;

	};

	class Code_Parameter_Type3
	{
	private:
		uint32_t m_uiVal_0 = 0;
		uint32_t m_uiVal_1 = 0;
		uint32_t m_uiVal_2 = 0;

	public:
		Code_Parameter_Type3();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};

	class Append_Script_Info
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
		Append_Script_Info();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};

	class Append_Script
	{
	private:
		uint32_t m_uiScriptCount = 0;
		std::vector<Append_Script_Info> m_vcInfo;

	public:
		Append_Script();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

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

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

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

		void Parse(uint8_t* pInfo);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

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

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		size_t GetCodeCount() const;
	};

	class Code
	{
	private:
		uint32_t m_uiCommand = 0;
		uint32_t m_uiVal_1 = 0;
		uint32_t m_uiVal_2 = 0;
		uint32_t m_uiVal_3 = 0;
		uint32_t m_uiVal_4 = 0;
		uint32_t m_uiSequnece = 0;
		uint32_t m_uiParameterType1Count = 0;
		uint32_t m_uiParameterType2Count = 0;
		uint32_t m_uiParameterType3Count = 0;

		Code_Parameter_Type0 m_ParameterType0;
		std::vector<Code_Parameter_Type1> m_vcParameterType1;
		std::vector<Code_Parameter_Type2> m_vcParameterType2;
		std::vector<Code_Parameter_Type3> m_vcParameterType3;

	public:
		Code();

		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		size_t GetCommand() const;
		Code_Parameter_Type0& GetParamType0();
	};


	class Parser
	{
	private:
		HDR m_HDR;
		std::vector<Code> m_vcCode;

	public:
		Parser();

		void Parse(std::wstring_view wsPath);
		void Parse(uint8_t* pData);
		Rut::RxMem::Auto Dump() const;
		Rut::RxJson::Value ToJson(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		std::vector<Code>& GetCodeList();
	};
}