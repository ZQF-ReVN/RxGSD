#include "GSP_Editor.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxMem.h"
#include "../../Rut/RxFile.h"


namespace GSD
{
	GSP::GSP(const std::wstring_view wsPackPath)
	{
		m_wsPackPath = wsPackPath;
	}

	void GSP::ReadIndex()
	{
		Rut::RxFile::Binary ifs{ this->m_wsPackPath, Rut::RIO_READ };

		uint32_t entry_count = ifs;
		for (size_t ite_entry = 0; ite_entry < entry_count; ite_entry++)
		{
			this->m_vcIndex.push_back(ifs);
		}
	}

	void GSP::Extract(const std::wstring& wsFolderPath)
	{
		Rut::RxFile::Binary ifs{ this->m_wsPackPath, Rut::RIO_READ };

		Rut::RxMem::Auto buffer;
		for (auto& entry : this->m_vcIndex)
		{
			buffer.SetSize(entry.uiSize);
			ifs.SetPos(entry.uiFOA, Rut::RIO_BEGIN);
			ifs.Read(buffer.GetPtr(), entry.uiSize);
			buffer.SaveData(wsFolderPath + Rut::RxStr::ToWCS(entry.aFileName, 932));
		}
	}
}