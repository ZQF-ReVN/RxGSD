#pragma once
#include <cstdint>


namespace ZQF::ReVN::RxGSD
{
#pragma pack(push,1)

	struct Struct
	{
		struct SPT
		{
			struct Char_Entry
			{
				std::uint32_t nType; // [0x7](normal char flag), [0xD](end string flag), [0x8](notation beg flag), [0x9](notation end flag)
				std::uint32_t nParam0;
				std::uint32_t nParan1;

				static constexpr inline auto SizeBytes{ sizeof(nType) + sizeof(nParam0) + sizeof(nParan1) };
			};

			struct Text_Entry
			{
				std::uint32_t uiCharCount;
				std::uint32_t aUn0[2];
				// SPT_Char aText[uiCharCount];
			};
		};

		using SPT_Char_Entry = SPT::Char_Entry;
		using SPT_Text_Entry = SPT::Text_Entry;


		struct BMZ
		{
			struct HDR
			{
				std::uint32_t nSignature; // ZLC3
				std::uint32_t uiDecompressSize; // BMP
				// zlib compress data

				static constexpr inline auto SizeBytes{ sizeof(nSignature) + sizeof(uiDecompressSize) };
			};

			struct ANIM
			{
				struct HDR
				{
					std::uint32_t nWidth;
					std::uint32_t nHeigh;
					std::uint32_t nEntryCnt;
					char aSignature[8];

					static constexpr inline auto SizeBytes{ sizeof(nWidth) + sizeof(nHeigh) + sizeof(nEntryCnt) + sizeof(aSignature) };
				};
			};
		};

		using BMZ_HDR = BMZ::HDR;
		using BMZ_ANIM_HDR = BMZ::ANIM::HDR;


		struct GSP
		{
			struct Entry
			{
				std::uint32_t nFOA;
				std::uint32_t nBytes;
				char aFileName[0x38];

				static constexpr inline auto SizeBytes{ sizeof(nFOA) + sizeof(nBytes) + sizeof(aFileName) };
			};

			//struct GSP::File
			//{
			//	uint32_t uiEntryCount;
			//	GSP::Entry aEntry[uiEntryCount];
			//  // data..
			//};
		};

		using GSP_Entry = GSP::Entry;
	};

#pragma pack(pop)
}
