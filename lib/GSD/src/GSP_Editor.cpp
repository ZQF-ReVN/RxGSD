#include "GSP_Editor.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxMem.h"
#include "../../Rut/RxPath.h"
#include "../../Rut/RxFile.h"


namespace GSD
{
	void GSP::Extract(const std::wstring_view wsPackPath, const std::wstring& wsFolderPath)
	{
		Rut::RxFile::Binary ifs{ wsPackPath, Rut::RIO_READ };

		std::vector<GSP_Entry> entry_list;
		{
			uint32_t entry_count = ifs;
			for (size_t ite_entry = 0; ite_entry < entry_count; ite_entry++)
			{
				entry_list.push_back(ifs);
			}
		}

		Rut::RxMem::Auto buffer;
		for (auto& entry : entry_list)
		{
			buffer.SetSize(entry.uiSize);
			ifs.SetPos(entry.uiFOA, Rut::RIO_BEGIN);
			ifs.Read(buffer.GetPtr(), entry.uiSize);
			buffer.SaveData(wsFolderPath + Rut::RxStr::ToWCS(entry.aFileName, 932));
		}
	}

	void GSP::Pack(const std::wstring_view wsPackPath, const std::wstring& wsFolderPath)
	{
		// Gen Index
		std::vector<GSP_Entry> entry_list;
		{
			std::vector<std::wstring> file_list;
			Rut::RxPath::CurFileNames(wsFolderPath, file_list, false);
			if (file_list.empty()) { throw std::runtime_error("GSP::Pack Not Files"); }

			uint32_t foa = (uint32_t)(4 + file_list.size() * sizeof(GSP_Entry));
			for (auto& file_name : file_list)
			{
				GSP_Entry entry = { 0 };
				entry.uiSize = (uint32_t)Rut::RxPath::GetFileSize(wsFolderPath + file_name);
				entry.uiFOA = foa;
				std::string file_name_dbcs = Rut::RxStr::ToMBCS(file_name, 932);
				memcpy(entry.aFileName, file_name_dbcs.data(), file_name_dbcs.size() + 1);

				entry_list.push_back(entry);

				foa += entry.uiSize;
			}
		}

		Rut::RxFile::Binary ofs{ wsPackPath, Rut::RIO_WRITE };

		// Write File Count
		uint32_t file_count = (uint32_t)entry_list.size();
		ofs.Write(&file_count, 4);

		// Write Index
		for (auto& entry : entry_list)
		{
			ofs.Write(&entry, sizeof(entry));
		}

		// Write Data
		for (auto& entry : entry_list)
		{
			Rut::RxMem::Auto file_data(wsFolderPath + Rut::RxStr::ToWCS(entry.aFileName, 932));
			ofs.Write(file_data.GetPtr(), file_data.GetSize());
		}
	}
}