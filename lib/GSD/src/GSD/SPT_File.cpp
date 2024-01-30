#include "SPT_File.h"


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
		this->Load(spt.GetPtr());
	}

	void File::Load(uint8_t* pData)
	{
		uint8_t* cur_ptr = pData;

		this->m_HDR.Load(cur_ptr);
		cur_ptr += this->m_HDR.GetSize();

		size_t code_count = this->m_HDR.GetCodeCount();
		for (size_t ite_chunk = 0; ite_chunk < code_count; ite_chunk++)
		{
			Code code;
			code.Load(cur_ptr);
			cur_ptr += code.GetSize();
			m_vcCode.emplace_back(std::move(code));
		}
	}

	void File::Load(Rut::RxJson::JValue& rfJson, size_t nCodePage)
	{
		m_HDR.Load(rfJson[L"Header"], nCodePage);

		Rut::RxJson::JArray& code_list = rfJson[L"Codes"];
		for (auto& code_json : code_list)
		{
			Code code;
			code.Load(code_json, nCodePage);
			m_vcCode.push_back(std::move(code));
		}
	}

	Rut::RxMem::Auto File::Make() const
	{
		Rut::RxMem::Auto mem_data;
		mem_data.SetSize(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();
		{
			Rut::RxMem::Auto hdr_mem = this->m_HDR.Make();
			memcpy(cur_ptr, hdr_mem.GetPtr(), hdr_mem.GetSize());
			cur_ptr += hdr_mem.GetSize();

			for (const auto& code : m_vcCode)
			{
				Rut::RxMem::Auto code_mem = code.Make();
				memcpy(cur_ptr, code_mem.GetPtr(), code_mem.GetSize());
				cur_ptr += code_mem.GetSize();
			}
		}
		return mem_data;
	}

	Rut::RxJson::JValue File::Make(size_t nCodePage) const
	{
		Rut::RxJson::JValue json;
		{
			json[L"Header"] = m_HDR.Make(nCodePage);

			Rut::RxJson::JArray& code_list = json[L"Codes"].ToAry();
			{
				for (const auto& code : m_vcCode)
				{
					code_list.emplace_back(code.Make(nCodePage));
				}
			}
		}
		return json;
	}

	std::vector<Code>& File::GetCodeList()
	{
		return m_vcCode;
	}

	size_t File::GetSize() const
	{
		size_t size = 0;
		{
			size += this->m_HDR.GetSize();
			for (const auto& code : m_vcCode)
			{
				size += code.GetSize();
			}
		}
		return size;
	}
}