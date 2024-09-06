#include "SPT_File.h"
#include <ranges>
#include <stdexcept>


namespace ZQF::RxGSD::SPT
{
	template <typename SPT_OBJ_T>
	static auto CheckDumpDataMatched(uint8_t* pOrg, SPT_OBJ_T& rfOBj) -> void
	{
		const auto obj_mem{ rfOBj.Make() };
		if (std::memcmp(pOrg, obj_mem.Ptr(), obj_mem.SizeBytes()))
		{
			throw std::runtime_error("Dump data mismatch");
		}
	}

	File::File()
	{

	}

	File::File(const std::string_view msSptPath)
	{
		this->Load(msSptPath);
	}

	File::File(const ZxJson::JValue& rfJson, const std::size_t nCodePage)
	{
		this->Load(rfJson, nCodePage);
	}

	auto File::Load(const std::string_view msSptPath) -> void
	{
		ZxMem spt{ msSptPath };
		this->Load(spt);
	}

	auto File::Load(ZxMem& vMem) -> void
	{
		m_HDR.Load(vMem);

		for ([[maybe_unused]] auto idx : std::views::iota(0u, m_HDR.GetCodeCount()))
		{
			m_vcCode.emplace_back(SPT::Code{ vMem });
		}
	}

	auto File::Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void
	{
		m_HDR.Load(rfJson.At("Header"), nCodePage);

		for (const auto& code_json : rfJson.At("Codes").GetArray()) 
		{ 
			m_vcCode.emplace_back(code_json, nCodePage);
		}
	}

	auto File::Make() const -> ZxMem
	{
		ZxMem mem_data{ this->SizeBytes() };
		
		mem_data << m_HDR.Make().Span();

		for (const auto& code : m_vcCode) 
		{ 
			code.Make(mem_data);
		}

		return mem_data.PosRewind();
	}

	auto File::Make(const std::size_t nCodePage) const -> ZxJson::JValue
	{
		ZxJson::JValue json;

		json["Header"].Assign(m_HDR.Make(nCodePage));

		auto& code_list = json["Codes"].ToArray();
		code_list.reserve(m_vcCode.size());
		for (auto& code : m_vcCode) 
		{
			ZxJson::JValue tmp;
			code.Make(tmp, nCodePage);
			code_list.push_back(std::move(tmp));
		}

		return json;
	}

	auto File::SizeBytes() const -> std::size_t
	{
		std::size_t bytes{ m_HDR.SizeBytes() };

		for (const auto& code : m_vcCode) 
		{ 
			bytes += code.SizeBytes();
		}

		return bytes;
	}

	auto File::GetCodeList() -> std::vector<SPT::Code>&
	{
		return m_vcCode;
	}
}