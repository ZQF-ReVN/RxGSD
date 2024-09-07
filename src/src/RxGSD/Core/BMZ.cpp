#include "BMZ.h"
#include "GSD_Types.h"
#include <zlib.h>
#include <ZxMem/ZxMem.h>
#include <ZxFile/ZxFile.h>
#include <stdexcept>


namespace ZQF::RxGSD
{
	auto BMZ::Export(const std::string_view msBMZPath, const std::string_view msBMPPath) -> void
	{
		ZxMem bmz_mem{ msBMZPath };
		const auto hdr_ptr{ bmz_mem.Ptr<BMZ_HDR*>() };

		auto bmp_org_size{ static_cast<uLongf>(hdr_ptr->uiDecompressSize) };
		const auto bmp_compress_data_size{ bmz_mem.SizeBytes<uLong>() - 0x8 };
		const auto bmp_compress_data_ptr{ bmz_mem.Ptr<Bytef*>() + 0x8 };

		ZxMem bmp_mem{ static_cast<std::size_t>(bmp_org_size) };
		auto bmp_buffer_ptr{ bmp_mem.Ptr<Bytef*>() };

		const auto status{ ::uncompress(bmp_buffer_ptr, &bmp_org_size, bmp_compress_data_ptr, bmp_compress_data_size) };
		if (status != Z_OK) { throw std::runtime_error(std::string{ "RxGSD::BMZ::Cryptor::Decode(): zlib decompress error! -> " }.append(msBMPPath)); }

		bmp_mem.Save(msBMPPath);
	}

	auto BMZ::Import(const std::string_view msBMZPath, const std::string_view msBMPPath, const std::string_view msSavePath) -> void
	{
		ZxMem cache;

		// cvt bmp to bmz
		ZxMem& bmp_mem{ cache.Load(msBMPPath) };
		auto bmz_data_buf_bytes{ bmp_mem.SizeBytes<uLong>() * 2 };
		ZxMem bmz_data_mem{ bmz_data_buf_bytes };
		const auto status{ ::compress(bmz_data_mem.Ptr<Bytef*>(), &bmz_data_buf_bytes, bmp_mem.Ptr<Bytef*>(), bmp_mem.SizeBytes<uLong>()) };
		if (status != Z_OK) { throw std::runtime_error(std::string{ "RxGSD::BMZ::Cryptor::Encode(): zlib compress error! -> " }.append(msBMPPath)); }
		bmz_data_mem.Resize(bmz_data_buf_bytes);

		// bmz hdr
		BMZ_HDR bmz_hdr
		{ 
			.nSignature{ 0x33434C5A }, // 'ZLC3'
			.uiDecompressSize{ bmp_mem.SizeBytes<std::uint32_t>() }
		};

		// check if a bmz has ANIM data
		ZxMem& anim_mem{ cache.Resize(0) };
		ZxFile raw_bmz_ifs{ msBMZPath,ZxFile::OpenMod::ReadSafe };
		// get raw bmz size bytes
		const auto raw_bmz_size_opt{ raw_bmz_ifs.Bytes() };
		if (raw_bmz_size_opt.has_value() == false) { throw std::runtime_error(std::string{ "RxGSD::BMZ::Cryptor::Encode(): get raw bmz size error! -> " }.append(msBMZPath)); }
		const auto raw_bmz_size{ *raw_bmz_size_opt };
		// read ANIM hdr
		raw_bmz_ifs.Seek(raw_bmz_size - BMZ_ANIM_HDR::SizeBytes, ZxFile::MoveWay::Set);
		const auto anim_hdr{ raw_bmz_ifs.Get<BMZ_ANIM_HDR>() };
		if (std::memcmp(anim_hdr.aSignature, "ANIMV450", sizeof(anim_hdr.aSignature)) == 0)
		{
			const auto anim_size{ static_cast<std::size_t>(anim_hdr.nEntryCnt) * 0x58 + BMZ_ANIM_HDR::SizeBytes };
			raw_bmz_ifs.Seek(raw_bmz_size - anim_size, ZxFile::MoveWay::Set);
			raw_bmz_ifs.Read(anim_mem.Resize(anim_size).Span());
		}

		// write data
		ZxFile ofs{ msSavePath, ZxFile::OpenMod::WriteForce };
		ofs << bmz_hdr << bmz_data_mem.Span();
		if (anim_mem.SizeBytes()) { ofs << anim_mem.Span(); }
	}
}