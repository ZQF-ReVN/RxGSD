#include "BMZ_Coder.h"
#include "GSD_Types.h"
#include "../../Rut/RxMem.h"

#include <zlib.h>
#include <stdexcept>


namespace GSD::BMZ
{
	void Coder::Decode(const std::wstring_view wsBMZ, const std::wstring_view wsBMP)
	{
		Rut::RxMem::Auto bmz{ wsBMZ };
		BMZ_HDR* bmz_ptr = (BMZ_HDR*)bmz.GetPtr();

		uLongf bmp_org_size = bmz_ptr->uiBMPSize;
		uLong bmp_compress_data_size = (uLong)(bmz.GetSize() - 0x8);
		Bytef* bmp_compress_data_ptr = (Bytef*)(bmz.GetPtr() + 0x8);

		Rut::RxMem::Auto bmp_buffer;
		bmp_buffer.SetSize(bmp_org_size);
		Bytef* bmp_buffer_ptr = (Bytef*)bmp_buffer.GetPtr();

		size_t status = ::uncompress(bmp_buffer_ptr, &bmp_org_size, bmp_compress_data_ptr, bmp_compress_data_size);
		status != Z_OK ? (throw std::runtime_error("ZLIB Decompress Error!")) : (void)(0);

		bmp_buffer.SaveData(wsBMP);
	}

	void Coder::Encode(const std::wstring_view wsBMP, const std::wstring_view wsBMZ)
	{
		Rut::RxMem::Auto bmp{ wsBMP };
		uLong bmp_size = (uLong)bmp.GetSize();
		Bytef* bmp_ptr = (Bytef*)bmp.GetPtr();

		Rut::RxMem::Auto bmz_buffer;
		uLongf bmz_buffer_size = bmp_size * 2 + 0x8;
		bmz_buffer.SetSize(bmz_buffer_size);
		Bytef* bmz_buffer_data_ptr = (Bytef*)(bmz_buffer.GetPtr() + 0x8);

		size_t status = ::compress(bmz_buffer_data_ptr, &bmz_buffer_size, bmp_ptr, bmp_size);
		status != Z_OK ? (throw std::runtime_error("ZLIB Compress Error!")) : (void)(0);

		bmz_buffer.SetSize((size_t)(bmz_buffer_size + 0x8));
		BMZ_HDR* bmz_ptr = (BMZ_HDR*)bmz_buffer.GetPtr();
		bmz_ptr->aSignature[0] = 'Z';
		bmz_ptr->aSignature[1] = 'L';
		bmz_ptr->aSignature[2] = 'C';
		bmz_ptr->aSignature[3] = '3';
		bmz_ptr->uiBMPSize = bmp_size;
		bmz_buffer.SaveData(wsBMZ);
	}
}