#pragma once
#include <ranges>

#include "SPT_HDR.h"
#include "SPT_Str.h"


namespace GSD::SPT
{
	class Global
	{
	private:
		Encryptor_Info m_EncryptorInfo;
		uint32_t m_uiUnFlag = 0;
		Append_Script m_aAppendScript[0xF];
		uint32_t m_uiGlobalStrCount = 0;
		std::vector<std::wstring> m_vcGlobalStr;
		uint8_t m_aUnData[0x60] = { 0 };

	public:
		Global()
		{

		}

		void Load(std::wstring_view wsPath, size_t nCodePage)
		{
			Rut::RxMem::Auto global_mem(wsPath);
			this->Load(global_mem.GetPtr(), nCodePage);
		}

		void Load(uint8_t* pData, size_t nCodePage)
		{
			uint8_t* cur_ptr = pData;

			m_EncryptorInfo.Load(pData);
			cur_ptr += m_EncryptorInfo.GetSize();

			m_uiUnFlag = *((uint32_t*)(cur_ptr));
			if (m_uiUnFlag != 0) { throw std::runtime_error("SPT_HDR: Unknow Format"); }
			cur_ptr += 4;

			for (auto& append_script : m_aAppendScript)
			{
				append_script.Load(cur_ptr);
				cur_ptr += append_script.GetSize();
			}

			m_uiGlobalStrCount = *((uint32_t*)(cur_ptr));
			cur_ptr += 4;

			for (auto ite_str : std::views::iota(0u, m_uiGlobalStrCount))
			{
				m_vcGlobalStr.emplace_back(SPT::Str::LoadANSI((char*)cur_ptr, nCodePage));
				cur_ptr += 260;
			}

			memcpy(m_aUnData, cur_ptr, 0x60);
		}

		const std::wstring& GetName(size_t nIndex) const
		{
			return m_vcGlobalStr.operator[](nIndex);
		}

	};
}