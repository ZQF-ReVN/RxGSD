#include "GSP.h"
#include "GSD_Types.h"
#include <ZxCvt/ZxCvt.h>
#include <ZxFS/Core.h>
#include <ZxFS/Searcher.h>
#include <ZxMem/ZxMem.h>
#include <ZxFile/ZxFile.h>
#include <vector>
#include <ranges>


namespace ZQF::RxGSD
{
	auto GSP::Export(const std::string_view msPackPath, const std::string_view msSavDir) -> void
	{
		ZxFile ifs{ msPackPath, ZxFile::OpenMod::ReadSafe };
		const auto entry_count{ ifs.Get<std::uint32_t>() };

		ZxMem index_table{ entry_count * RxGSD::GSP_Entry::SizeBytes };
		ifs.Read(index_table.Span());

		ZxCvt cvt;
		ZxMem cache;
		std::string save_path{ msSavDir };
		for (const auto& entry : index_table.Span<RxGSD::GSP_Entry>())
		{
			ifs.Read(cache.Resize(entry.nBytes).Span());
			cache.Save(save_path.append(cvt.MBCSToUTF8(entry.aFileName, 932)));
			save_path.resize(msSavDir.size());
		}
	}

	auto GSP::Import(const std::string_view msFilesDir, const std::string_view msSavPath) -> void
	{
		std::vector<std::string> file_path_vec{ ZxFS::Searcher::GetFilePaths(msFilesDir, true, false) };
		if (file_path_vec.empty()) { throw std::runtime_error(std::string{ "RxGSD::GSP::Import(): not find files! -> " }.append(msFilesDir)); }

		ZxMem index_table_mem{ file_path_vec.size() * GSP_Entry::SizeBytes };
		const auto index_table_sp{ index_table_mem.Span<RxGSD::GSP_Entry>() };
		{
			ZxCvt cvt;
			auto foa{ static_cast<std::uint32_t>(4 + index_table_mem.SizeBytes()) };
			for (auto&& [file_path, entry] : std::views::zip(file_path_vec, index_table_sp))
			{
				entry.nFOA = foa;
				entry.nBytes = static_cast<std::uint32_t>(ZxFS::FileSize(file_path).value());
				const auto file_name_dbcs{ cvt.UTF8ToMBCS(ZxFS::FileName(file_path), 932) };
				std::memcpy(entry.aFileName, file_name_dbcs.data(), file_name_dbcs.size() + 1);
				foa += entry.nBytes;
			}
		}

		ZxFile ofs{ msSavPath, ZxFile::OpenMod::WriteForce };

		ofs << static_cast<std::uint32_t>(index_table_sp.size());
		ofs << index_table_sp;

		ZxMem& cache{ index_table_mem };
		for (const auto& file_path : file_path_vec)
		{
			ofs.Write(cache.Load(file_path).Span());
		}
	}
}