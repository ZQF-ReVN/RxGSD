#include "SPT_File.h"

#include <ranges>


namespace GSD::SPT
{
	File::File()
	{

	}

	File::File(const std::filesystem::path& phSpt)
	{
		this->Load(phSpt);
	}

	void File::Load(const std::filesystem::path& phSpt)
	{
		Rut::RxMem::Auto spt{ phSpt };
		Rut::RxMem::View view = spt.GetView();
		this->Load(view);
	}

	void File::Load(Rut::RxMem::View& vMem)
	{
		m_HDR.Load(vMem);
		m_vcCode.append_range(std::views::repeat(Code(vMem), m_HDR.GetCodeCount()));
	}

	void File::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_HDR.Load(rfJson[L"Header"], nCodePage);
		std::ranges::for_each(rfJson[L"Codes"].ToAry(), [this, &nCodePage](auto& code_json) { m_vcCode.emplace_back(code_json, nCodePage); });
	}

	Rut::RxMem::Auto File::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		Rut::RxMem::View view = mem_data.GetView();
		view << m_HDR.Make();
		std::ranges::for_each(m_vcCode, [&view](auto& code) { view << code.Make(); });
		return mem_data;
	}

	Rut::RxJson::JValue File::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		json[L"Header"] = m_HDR.Make(nCodePage);
		Rut::RxJson::JArray& code_list = json[L"Codes"].ToAry();
		std::ranges::for_each(m_vcCode, [&code_list, &nCodePage](auto& code) { code_list.emplace_back(code.Make(nCodePage)); });
		return json;
	}

	std::vector<Code>& File::GetCodeList()
	{
		return m_vcCode;
	}

	size_t File::GetSize() const
	{
		size_t size = m_HDR.GetSize();
		std::ranges::for_each(m_vcCode, [&size](auto& code) { size += code.GetSize(); });
		return size;
	}
}