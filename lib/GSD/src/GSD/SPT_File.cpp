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

		for (auto i : std::views::iota(0u, m_HDR.GetCodeCount()))
		{
			m_vcCode.emplace_back(vMem);
		}
	}

	void File::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_HDR.Load(rfJson[L"Header"], nCodePage);

		for (auto& code_json : rfJson[L"Codes"].ToAry()) 
		{ 
			m_vcCode.emplace_back(code_json, nCodePage);
		}
	}

	Rut::RxMem::Auto File::Make() const
	{
		Rut::RxMem::Auto mem_data(this->GetSize());
		Rut::RxMem::View view = mem_data.GetView();

		view << m_HDR.Make();

		Rut::RxMem::Auto buffer;
		for (auto& code : m_vcCode) 
		{ 
			code.Make(buffer);
			view << buffer;
		}

		return mem_data;
	}

	Rut::RxJson::JValue File::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;

		json[L"Header"] = m_HDR.Make(nCodePage);

		Rut::RxJson::JArray& code_list = json[L"Codes"].ToAry();
		code_list.reserve(m_vcCode.size());
		for (auto& code : m_vcCode) 
		{
			Rut::RxJson::JValue tmp;
			code.Make(tmp, nCodePage);
			code_list.push_back(std::move(tmp));
		}

		return json;
	}

	std::vector<Code>& File::GetCodeList()
	{
		return m_vcCode;
	}

	size_t File::GetSize() const
	{
		size_t size = m_HDR.GetSize();

		for (auto& code : m_vcCode) 
		{ 
			size += code.GetSize();
		}

		return size;
	}
}