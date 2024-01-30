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
		std::vector<std::string> m_vcGlobalStr;
		uint8_t m_aUnData[0x60] = { 0 };

	public:
		Global()
		{

		}

		void Load(const std::filesystem::path& phGlobal)
		{
			Rut::RxMem::Auto global_mem(phGlobal);
			this->Load(global_mem.GetPtr());
		}

		void Load(uint8_t* pData)
		{
			uint8_t* cur_ptr = pData;

			m_EncryptorInfo.Load(pData);
			cur_ptr += m_EncryptorInfo.GetSize();

			m_uiUnFlag = *((uint32_t*)(cur_ptr));
			if (m_uiUnFlag != 0) { throw std::runtime_error("SPT_Global: Unknow Format"); }
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
				m_vcGlobalStr.emplace_back((char*)cur_ptr);
				cur_ptr += 260;
			}

			memcpy(m_aUnData, cur_ptr, 0x60);
		}

		std::vector<std::wstring> GetStrTable(size_t nCodePage) const
		{
			std::vector<std::wstring> str_table;
			for (const auto& str : m_vcGlobalStr)
			{
				str_table.emplace_back(Str::LoadANSI(str, nCodePage));
			}
			return str_table;
		}
	};
}