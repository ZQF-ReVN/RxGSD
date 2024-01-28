#pragma once
#include <string>
#include <vector>

#include "GSD_Types.h"
#include "RxMem.h"
#include "RxJson.h"


namespace GSD::SPT
{
	class Arg_Type0
	{
	private:
		uint32_t m_uiNameSeq0 = 0;
		uint32_t m_uiNameSeq1 = 0;
		uint32_t m_uiUn2 = 0;
		uint32_t m_uiUn3 = 0;
		uint32_t m_uiStrType0Len = 0; // dbcs char count
		uint32_t m_uiStrType1Len = 0;
		uint32_t m_uiStrType2Len = 0;

		std::vector<SPT_Char_Entry> m_vcStrType0CharList;
		std::string m_msStrType1;
		std::string m_msStrType2;

	public:
		Arg_Type0();

		void Load(uint8_t* pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		void SetType0Text(const std::string& msText);
		void SetType1Text(const std::string& msText);
		void SetType2Text(const std::string& msText);
		size_t GetType0TextLen()const;
		std::string GetType0Text() const;
		std::wstring GetType0Text(size_t nCodePage) const;
		size_t GetNameSeq() const;
		size_t GetSize() const;
	};

	class Arg_Type1
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
		Arg_Type1();

		void Load(uint8_t* pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		void SetStr(std::string_view msStr);
	};

	class Arg_Type2
	{
	private:
		uint32_t m_uiParameterType1Count = 0;
		std::vector<Arg_Type1> m_vcParameterType1;

	public:
		Arg_Type2();

		void Load(uint8_t* pData);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;

	};

	class Arg_Type3
	{
	private:
		uint32_t m_uiVal_0 = 0;
		uint32_t m_uiVal_1 = 0;
		uint32_t m_uiVal_2 = 0;

	public:
		Arg_Type3();

		void Load(uint8_t* pData);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
	};
}