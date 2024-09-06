#include "SPT_Cryptor.h"
#include <cmath>


namespace ZQF::RxGSD::SPT
{
	static std::uint32_t sg_aTable[128] =
	{
		0x00000005, 0x00000001, 0xFFFFFFFE, 0x00000000, 0x00000002, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFA,
		0x00000006, 0x00000003, 0x00000000, 0xFFFFFFFD, 0x00000003, 0xFFFFFFFC, 0xFFFFFFFD, 0xFFFFFFFE,
		0x00000007, 0x00000005, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF9,
		0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD,
		0x00000002, 0x00000003, 0x00000005, 0x00000002, 0xFFFFFFFD, 0x00000001, 0xFFFFFFFA, 0xFFFFFFFC,
		0x00000004, 0x00000004, 0x00000001, 0x00000004, 0x00000002, 0xFFFFFFFB, 0xFFFFFFFC, 0xFFFFFFFA,
		0x00000001, 0x00000002, 0x00000002, 0x00000003, 0x00000001, 0x00000002, 0xFFFFFFFC, 0xFFFFFFF9,
		0x00000002, 0xFFFFFFFF, 0x00000001, 0x00000004, 0x00000002, 0xFFFFFFFF, 0xFFFFFFFB, 0xFFFFFFFE,
		0x00000002, 0x00000006, 0xFFFFFFFF, 0x00000000, 0x00000001, 0xFFFFFFFB, 0xFFFFFFFE, 0xFFFFFFFF,
		0x00000003, 0x00000004, 0x00000000, 0x00000003, 0xFFFFFFFD, 0x00000002, 0xFFFFFFFA, 0xFFFFFFFD,
		0x00000007, 0x00000005, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF9,
		0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD, 0x00000003, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFD,
		0x00000006, 0x00000003, 0xFFFFFFFE, 0x00000004, 0xFFFFFFFD, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFB,
		0x00000005, 0x00000006, 0x00000004, 0xFFFFFFFF, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFE, 0xFFFFFFFC,
		0x00000007, 0xFFFFFFFF, 0x00000004, 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFD, 0xFFFFFFFE,
		0x00000001, 0x00000005, 0xFFFFFFFE, 0xFFFFFFFF, 0x00000001, 0x00000002, 0xFFFFFFFE, 0xFFFFFFFC
	};


	auto Cryptor::DecodeRound0(const std::span<std::uint8_t> spEnc, const std::size_t nType) -> void
	{
		switch (nType)
		{
		case 0:
		{
			const auto dec_size{ spEnc.size() - 2 };
			for (std::size_t ite_bytes{}; ite_bytes <= dec_size; ite_bytes += 2)
			{
				const auto tmp_byte_0{ spEnc[ite_bytes + 0] };
				const auto tmp_byte_1{ spEnc[ite_bytes + 1] };
				spEnc[ite_bytes + 0] = tmp_byte_1;
				spEnc[ite_bytes + 1] = tmp_byte_0;
			}
		}
		break;

		case 1:
		{
			const auto dec_size{ spEnc.size() - 4 };
			for (std::size_t ite_bytes{}; ite_bytes <= dec_size; ite_bytes += 4)
			{
				const auto tmp_byte_0{ spEnc[ite_bytes + 0] };
				const auto tmp_byte_1{ spEnc[ite_bytes + 1] };
				const auto tmp_byte_2{ spEnc[ite_bytes + 2] };
				const auto tmp_byte_3{ spEnc[ite_bytes + 3] };
				spEnc[ite_bytes + 0] = tmp_byte_2;
				spEnc[ite_bytes + 1] = tmp_byte_3;
				spEnc[ite_bytes + 2] = tmp_byte_0;
				spEnc[ite_bytes + 3] = tmp_byte_1;
			}
		}
		break;

		case 2:
		{
			const auto dec_size{ spEnc.size() - 8 };
			for (std::size_t ite_bytes{}; ite_bytes <= dec_size; ite_bytes += 8)
			{
				const auto tmp_byte_0{ spEnc[ite_bytes + 0] };
				const auto tmp_byte_1{ spEnc[ite_bytes + 1] };
				const auto tmp_byte_2{ spEnc[ite_bytes + 2] };
				const auto tmp_byte_3{ spEnc[ite_bytes + 3] };
				const auto tmp_byte_4{ spEnc[ite_bytes + 4] };
				const auto tmp_byte_5{ spEnc[ite_bytes + 5] };
				const auto tmp_byte_6{ spEnc[ite_bytes + 6] };
				const auto tmp_byte_7{ spEnc[ite_bytes + 7] };
				spEnc[ite_bytes + 0] = tmp_byte_6;
				spEnc[ite_bytes + 1] = tmp_byte_4;
				spEnc[ite_bytes + 2] = tmp_byte_5;
				spEnc[ite_bytes + 3] = tmp_byte_7;
				spEnc[ite_bytes + 4] = tmp_byte_1;
				spEnc[ite_bytes + 5] = tmp_byte_2;
				spEnc[ite_bytes + 6] = tmp_byte_0;
				spEnc[ite_bytes + 7] = tmp_byte_3;
			}
		}
		break;
		}
	}

	auto Cryptor::DecodeRound1(const std::uint32_t* pTable, const std::span<std::uint8_t> spEnc, const std::size_t nStart) -> void
	{
		if (nStart >= 8) { return; }

		for (auto& byte : spEnc)
		{
			std::uint8_t decode_byte{};
			for (std::size_t ite_table{}; ite_table < 8; ++ite_table)
			{
				const auto shift_value{ pTable[8 * nStart + 64 + ite_table] };
				if ((int)shift_value <= -1)
				{
					decode_byte |= ((1 << ite_table) & byte) >> std::abs(static_cast<int>(shift_value));
				}
				else
				{
					decode_byte |= ((1 << ite_table) & byte) << shift_value;
				}
			}
			byte = ~decode_byte;
		}
	}


	auto Cryptor::Decode(const std::span<std::uint8_t> spSpt, const bool isMakeReadable) -> void
	{
		const auto start_index{ spSpt[0] ^ 0xF0 };
		const auto decode_type{ spSpt[1] ^ 0xF0 };

		const auto enc_data_ptr{ spSpt.data() + 0x4 };
		const auto enc_data_size{ spSpt.size() - 0x4 };

		Cryptor::DecodeRound0({ enc_data_ptr, enc_data_size }, decode_type);
		Cryptor::DecodeRound1(SPT::sg_aTable, { enc_data_ptr, enc_data_size }, start_index);

		if (isMakeReadable)
		{
			spSpt[0] = 0xFF;
			spSpt[1] = 0xFF;
		}
	}
}