#pragma once
#include <string>
#include <vector>

#include "GSD_Types.h"
#include "SPT_Arg.h"

#include "Rut/RxMem.h"
#include "Rut/RxJson.h"


namespace GSD::SPT
{
	class Code
	{
	private:
		uint32_t m_uiCommand = 0;
		uint32_t m_uiVal_1 = 0;
		uint32_t m_uiVal_2 = 0;
		uint32_t m_uiVal_3 = 0;
		uint32_t m_uiVal_4 = 0;
		uint32_t m_uiSequnece = 0;
		uint32_t m_uiArgType1Count = 0;
		uint32_t m_uiArgType2Count = 0;
		uint32_t m_uiArgType3Count = 0;

		Arg_Type0 m_ArgType0;
		std::vector<Arg_Type1> m_vcArgType1;
		std::vector<Arg_Type2> m_vcArgType2;
		std::vector<Arg_Type3> m_vcArgType3;

	public:
		Code();

		void Load(uint8_t* pData);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		size_t GetCommand() const;
		Arg_Type0& GetArgType0();
	};
}