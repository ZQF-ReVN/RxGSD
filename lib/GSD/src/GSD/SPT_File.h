#pragma once
#include "SPT_HDR.h"
#include "SPT_Code.h"

#include <stdexcept>


namespace GSD::SPT
{
	template <typename SPT_OBJ_T>
	void CheckDumpDataMatched(uint8_t* pOrg, SPT_OBJ_T& rfOBj)
	{
		Rut::RxMem::Auto obj_mem = rfOBj.Make();
		if (memcmp(pOrg, obj_mem.GetPtr(), obj_mem.GetSize()))
		{
			throw std::runtime_error("Dump data mismatch");
		}
	}


	class File
	{
	private:
		HDR m_HDR;
		std::vector<Code> m_vcCode;

	public:
		File();
		File(const std::filesystem::path& phSpt);

		void Load(const std::filesystem::path& phSpt);
		void Load(Rut::RxMem::View& vMem);
		void Load(Rut::RxJson::JValue& rfJson, size_t nCodePage);
		Rut::RxMem::Auto Make() const;
		Rut::RxJson::JValue Make(size_t nCodePage) const;

	public:
		size_t GetSize() const;
		std::vector<Code>& GetCodeList();
	};
}