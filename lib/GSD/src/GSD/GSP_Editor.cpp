#include "GSP_Editor.h"
#include "Rut/RxMem.h"
#include "Rut/RxStr.h"
#include "Rut/RxFile.h"


namespace GSD
{
	void GSP::Extract(const std::filesystem::path& phPack, const std::filesystem::path& phFolder)
	{
		// Open Pack File Binary Stream
		Rut::RxFile::Binary ifs{ phPack, Rut::RIO_READ };

		// Read Entries
		std::vector<GSP_Entry> entry_list;
		{
			uint32_t entry_count = ifs.Get<uint32_t>();
			for (size_t ite_entry = 0; ite_entry < entry_count; ite_entry++)
			{
				entry_list.push_back(ifs.Get<GSP_Entry>());
			}
		}

		// Extract Files
		Rut::RxMem::Auto buffer;
		for (auto& entry : entry_list)
		{
			buffer.ReadData(ifs, entry.uiSize, entry.uiFOA);
			buffer.SaveData(phFolder / Rut::RxStr::ToWCS(entry.aFileName, 932));
		}
	}

	void GSP::Pack(const std::filesystem::path& phFolder, const std::filesystem::path& phPack)
	{
		// Gen Index
		std::vector<GSP_Entry> entry_list;
		{
			// Fnid Files
			std::vector<std::filesystem::path> file_path_list;
			for (auto& path_entry : std::filesystem::directory_iterator(phFolder))
			{
				if (path_entry.is_regular_file() == false) { continue; }
				file_path_list.emplace_back(path_entry);
			}
			if (file_path_list.empty()) { throw std::runtime_error("GSP::Pack Not Files"); }

			// Set Beg File Offset
			uint32_t foa = (uint32_t)(4 + file_path_list.size() * sizeof(GSP_Entry)); //skip hdr

			// Make Index
			for (auto& file_path : file_path_list)
			{
				GSP_Entry entry = { 0 };
				entry.uiSize = (uint32_t)std::filesystem::file_size(file_path);
				entry.uiFOA = foa;
				std::string file_name_dbcs = Rut::RxStr::ToMBCS(file_path.filename().wstring(), 932);
				memcpy(entry.aFileName, file_name_dbcs.data(), file_name_dbcs.size() + 1);

				entry_list.emplace_back(entry);
				foa += entry.uiSize;
			}
		}

		// Create Pack File Binary Stream
		Rut::RxFile::Binary ofs{ phPack, Rut::RIO_WRITE };

		// Write File Count
		ofs << (uint32_t)entry_list.size();

		// Write Index
		for (auto& entry : entry_list)
		{
			ofs << entry;
		}

		// Write Data
		Rut::RxMem::Auto tmp_buffer;
		for (auto& entry : entry_list)
		{
			tmp_buffer.LoadFile(phFolder / Rut::RxStr::ToWCS(entry.aFileName, 932));
			tmp_buffer.WriteData(ofs);
		}
	}
}