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
			Rut::RxMem::View global_view = global_mem.GetView();
			this->Load(global_view);
		}

		void Load(Rut::RxMem::View& vMem)
		{
			m_EncryptorInfo.Load(vMem);

			vMem >> m_uiUnFlag;

			if (m_uiUnFlag != 0) { throw std::runtime_error("SPT_Global: Unknow Format"); }

			for (auto& append_script : m_aAppendScript) 
			{ 
				append_script.Load(vMem);
			}

			vMem >> m_uiGlobalStrCount;

			for (auto ite_str : std::views::iota(0u, m_uiGlobalStrCount))
			{
				m_vcGlobalStr.emplace_back(vMem.CurPtr<char>());
				vMem.Skip(260);
			}

			vMem >> m_aUnData;
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