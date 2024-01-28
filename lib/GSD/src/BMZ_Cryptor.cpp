#include "BMZ_Cryptor.h"
#include "GSD_Types.h"
#include "RxMem.h"

#include <zlib.h>
#include <stdexcept>


namespace GSD::BMZ
{
	void Cryptor::Decode(const std::wstring_view wsBMZ, const std::wstring_view wsBMP)
	{
		Rut::RxMem::Auto bmz_mem{ wsBMZ };
		GSD_BMZ_HDR* hdr_ptr = (GSD_BMZ_HDR*)bmz_mem.GetPtr();

		uLongf bmp_org_size = hdr_ptr->uiDecompressSize;
		uLong bmp_compress_data_size = (uLong)(bmz_mem.GetSize() - 0x8);
		Bytef* bmp_compress_data_ptr = (Bytef*)(bmz_mem.GetPtr() + 0x8);

		Rut::RxMem::Auto bmp_mem((size_t)bmp_org_size);
		Bytef* bmp_buffer_ptr = (Bytef*)bmp_mem.GetPtr();

		size_t status = ::uncompress(bmp_buffer_ptr, &bmp_org_size, bmp_compress_data_ptr, bmp_compress_data_size);
		status != Z_OK ? (throw std::runtime_error("ZLIB Decompress Error!")) : (void)(0);

		bmp_mem.SaveData(wsBMP);
	}

	void Cryptor::Encode(const std::wstring_view wsOrgBMZPath, const std::wstring_view wsBMPPath, const std::wstring_view wsNewBMZPath)
	{
		Rut::RxMem::Auto anim_mem{ wsOrgBMZPath };
		uint8_t* anim_signature_ptr = anim_mem.GetPtr() + anim_mem.GetSize() - 8;
		if (memcmp((char*)anim_signature_ptr, "ANIMV450", 8) == 0)
		{
			uint32_t entry_count = *(uint32_t*)(anim_signature_ptr - 0x4);
			uint32_t width = *(uint32_t*)(anim_signature_ptr - 0x8);
			uint32_t heigh = *(uint32_t*)(anim_signature_ptr - 0xC);
			uint32_t index_size = entry_count * 0x58;
			uint32_t anim_size = index_size + 4 + 4 + 4 + 8;
			uint8_t* anim_ptr = anim_signature_ptr - index_size - 4 - 4 - 4;
			anim_mem.SetSize(anim_size);
			memcpy(anim_mem.GetPtr(), anim_ptr, anim_size);
		}
		else
		{
			anim_mem.SetSize(0);
		}

		Rut::RxMem::Auto bmp_mem{ wsBMPPath };
		uLong bmp_size = (uLong)bmp_mem.GetSize();
		Bytef* bmp_ptr = (Bytef*)bmp_mem.GetPtr();

		Rut::RxMem::Auto bmz_mem;
		uLongf bmz_buffer_size = bmp_size * 2 + 0x8;
		bmz_mem.SetSize(bmz_buffer_size);
		Bytef* bmz_buffer_ptr = (Bytef*)(bmz_mem.GetPtr() + 0x8);

		size_t status = ::compress(bmz_buffer_ptr, &bmz_buffer_size, bmp_ptr, bmp_size);
		status != Z_OK ? (throw std::runtime_error("ZLIB Compress Error!")) : (void)(0);

		bmz_mem.SetSize((size_t)(0x8 + bmz_buffer_size + anim_mem.GetSize()));
		GSD_BMZ_HDR* bmz_ptr = (GSD_BMZ_HDR*)bmz_mem.GetPtr();
		bmz_ptr->aSignature[0] = 'Z';
		bmz_ptr->aSignature[1] = 'L';
		bmz_ptr->aSignature[2] = 'C';
		bmz_ptr->aSignature[3] = '3';
		bmz_ptr->uiDecompressSize = bmp_size;

		if (anim_mem.GetSize())
		{
			memcpy(bmz_mem.GetPtr() + bmz_buffer_size + 0x8, anim_mem.GetPtr(), anim_mem.GetSize());
		}

		bmz_mem.SaveData(wsNewBMZPath);
	}
}