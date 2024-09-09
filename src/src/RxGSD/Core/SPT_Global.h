#pragma once
#include <ranges>
#include <Zut/ZxCvt.h>
#include <RxGSD/Core/SPT_HDR.h>


namespace ZQF::RxGSD::SPT
{
	class Global
	{
	private:
		Encryptor_Info m_EncryptorInfo;
		std::uint32_t m_uiUnFlag{};
		Append_Script m_aAppendScript[0xF];
		std::uint32_t m_uiGlobalStrCount{};
		std::vector<std::string> m_vcGlobalStr;
		std::uint8_t m_aUnData[0x60]{};

	public:
		Global()
		{

		}

		auto Load(const std::string_view msGlobalPath) -> void
		{
			ZxMem global_mem{ msGlobalPath };
			this->Load(global_mem);
		}

		auto Load(ZxMem& vMem) -> void
		{
			m_EncryptorInfo.Load(vMem);

			vMem >> m_uiUnFlag;

			if (m_uiUnFlag != 0) { throw std::runtime_error("SPT_Global: Unknow Format"); }

			for (auto& append_script : m_aAppendScript) 
			{ 
				append_script.Load(vMem);
			}

			vMem >> m_uiGlobalStrCount;

			for ([[maybe_unused]] auto idx : std::views::iota(0u, m_uiGlobalStrCount))
			{
				m_vcGlobalStr.emplace_back(vMem.PtrCur<char*>());
				vMem.PosInc(260);
			}

			vMem >> m_aUnData;
		}

		auto GetStrTable(const std::size_t nCodePage) const -> std::vector<std::string>
		{
			std::vector<std::string> str_table;

			ZxCvt cvt;
			for (const auto& str : m_vcGlobalStr) 
			{
				str_table.emplace_back(cvt.MBCSToUTF8(str, nCodePage));
			}

			return str_table;
		}
	};
}