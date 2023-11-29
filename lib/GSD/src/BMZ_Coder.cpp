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

		uLong bmp_size = bmz_ptr->uiBMPSize;

		Rut::RxMem::Auto bmp_buffer;
		bmp_buffer.SetSize(bmp_size);

		size_t status = ::uncompress(bmp_buffer.GetPtr(), &bmp_size, bmz.GetPtr() + 0x8, (uLong)(bmz.GetPtr() - 0x8));
		status == Z_OK ? (bmp_buffer.SaveData(wsBMP)) : (throw std::runtime_error("ZLIB Decompress Error!"));
	}

	void Coder::Encode(const std::wstring_view wsBMP, const std::wstring_view wsBMZ)
	{
		Rut::RxMem::Auto bmp{ wsBMP };
		uLong bmp_size = bmp.GetSize();

		uLong bmz_buffer_size = bmp_size * 2 + 0x8;
		Rut::RxMem::Auto bmz_buffer;
		bmz_buffer.SetSize(bmz_buffer_size);

		size_t status = ::compress(bmz_buffer.GetPtr() + 0x8, &bmz_buffer_size, bmp.GetPtr(), bmp_size);
		if (status == Z_OK)
		{
			bmz_buffer.SetSize(bmz_buffer_size + 0x8);
			BMZ_HDR* bmz_ptr = (BMZ_HDR*)bmz_buffer.GetPtr();
			bmz_ptr->aSignature[0] = 'Z';
			bmz_ptr->aSignature[1] = 'L';
			bmz_ptr->aSignature[2] = 'C';
			bmz_ptr->aSignature[3] = '3';
			bmz_ptr->uiBMPSize = bmp_size;
			bmz_buffer.SaveData(wsBMZ);
		}
		else
		{
			throw std::runtime_error("ZLIB Compress Error!");
		}
	}
}