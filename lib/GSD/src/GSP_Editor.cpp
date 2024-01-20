#include "GSP_Editor.h"
#include "../../Rut/RxMem.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxFile.h"

#include <filesystem>


namespace GSD
{
	void GSP::Extract(const std::wstring_view wsPackPath, const std::wstring& wsFolderPath)
	{
		Rut::RxFile::Binary ifs{ wsPackPath, Rut::RIO_READ };

		std::vector<GSP_Entry> entry_list;
		{
			uint32_t entry_count = ifs.Get<uint32_t>();
			for (size_t ite_entry = 0; ite_entry < entry_count; ite_entry++)
			{
				entry_list.push_back(ifs.Get<GSP_Entry>());
			}
		}

		Rut::RxMem::Auto buffer;
		for (auto& entry : entry_list)
		{
			buffer.ReadData(ifs, entry.uiSize, entry.uiFOA);
			buffer.SaveData(wsFolderPath + Rut::RxStr::ToWCS(entry.aFileName, 932));
		}
	}

	void GSP::Pack(const std::wstring_view wsPackPath, const std::wstring& wsFolderPath)
	{
		// Gen Index
		std::vector<GSP_Entry> entry_list;
		{
			size_t file_count = 0;
			for (auto& entry : std::filesystem::directory_iterator(wsFolderPath))
			{
				if (entry.is_regular_file() == false) { continue; }
				file_count++;
			}

			if (file_count) { throw std::runtime_error("GSP::Pack Not Files"); }

			uint32_t foa = (uint32_t)(4 + file_count * sizeof(GSP_Entry)); //skip hdr

			for (auto& entry : std::filesystem::directory_iterator(wsFolderPath))
			{
				if (entry.is_regular_file() == false) { continue; }

				const std::filesystem::path& file_path = entry.path();

				GSP_Entry entry = { 0 };
				entry.uiSize = (uint32_t)std::filesystem::file_size(file_path);
				entry.uiFOA = foa;
				std::string file_name_dbcs = Rut::RxStr::ToMBCS(file_path.filename().wstring(), 932);
				memcpy(entry.aFileName, file_name_dbcs.data(), file_name_dbcs.size() + 1);

				entry_list.push_back(entry);

				foa += entry.uiSize;
			}
		}

		Rut::RxFile::Binary ofs{ wsPackPath, Rut::RIO_WRITE };

		// Write File Count
		uint32_t entry_count = (uint32_t)entry_list.size();
		ofs << entry_count;

		// Write Index
		for (auto& entry : entry_list)
		{
			ofs << entry;
		}

		// Write Data
		Rut::RxMem::Auto file_buffer;
		for (auto& entry : entry_list)
		{
			file_buffer.LoadFile(wsFolderPath + Rut::RxStr::ToWCS(entry.aFileName, 932));
			file_buffer.WriteData(ofs);
		}
	}
}