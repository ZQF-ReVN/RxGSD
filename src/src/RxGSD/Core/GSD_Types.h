#pragma once
#include <cstdint>
#include <Windows.h>


namespace ZQF::RxGSD::SPT
{
#pragma pack(push,1)
	struct Char_Entry
	{
		std::uint32_t nType; // [0x7](normal char flag), [0xD](end string flag), [0x8](notation beg flag), [0x9](notation end flag)
		std::uint32_t nParam0;
		std::uint32_t nParan1;

		static constexpr inline auto SizeBytes{ sizeof(nType) + sizeof(nParam0) + sizeof(nParan1) };
	};
#pragma pack(pop)
}


namespace ZQF::RxGSD
{
#pragma pack(push,1)
	struct GSP_Entry
	{
		std::uint32_t nFOA;
		std::uint32_t nBytes;
		char aFileName[0x38];

		static constexpr inline auto SizeBytes{ sizeof(nFOA) + sizeof(nBytes) + sizeof(aFileName) };
	};

	//struct GSP_File
	//{
	//	uint32_t uiEntryCount;
	//	GSP_Entry aEntry[uiEntryCount];
	//  // data..
	//};

	struct BMZ_HDR
	{
		std::uint32_t nSignature; // ZLC3
		std::uint32_t uiDecompressSize; // BMP
		// zlib compress data

		static constexpr inline auto SizeBytes{ sizeof(nSignature) + sizeof(uiDecompressSize) };
	};

	struct BMZ_ANIM_HDR
	{
		std::uint32_t nWidth;
		std::uint32_t nHeigh;
		std::uint32_t nEntryCnt;
		char aSignature[8];

		static constexpr inline auto SizeBytes{ sizeof(nWidth) + sizeof(nHeigh) + sizeof(nEntryCnt) + sizeof(aSignature) };
	};

	// version 3.0.2.9
	struct GSD_STD_String_T0
	{
		union
		{
			char* cpStr;
			char aStr[16];
		};
		uint32_t uiLen;
		uint32_t uiReserve;
		uint32_t uiAlig;

		char* GetRawStr()
		{
			return this->uiLen >= 16 ? this->cpStr : this->aStr;
		}
	};

	// version 3.0.1.8
	struct GSD_STD_String_T1
	{
		uint32_t uiAlig;
		union
		{
			char* cpStr;
			char aStr[16];
		};
		uint32_t uiLen;
		uint32_t uiReserve;

		char* GetRawStr()
		{
			return this->uiLen >= 16 ? this->cpStr : this->aStr;
		}
	};

	struct GSD_FS_Pack_Entry
	{
		uint64_t uiFOA;
		uint64_t uiSize;
		char aFileName[112];
	};

	struct GSD_FS
	{
		HANDLE hFile;
		uint32_t uiFilePointer_Back;
		uint32_t hFile_0;
		LARGE_INTEGER sOldFilePointer;
		uint32_t aUn0[3];
		LARGE_INTEGER sFilePointer;
		LARGE_INTEGER sFileSize;
		uint32_t hFile_1;
		uint32_t aUn1[28];
		uint32_t hFile_2;
		uint32_t aUn2[6];
		uint32_t hFile_3;
		uint32_t aUn3[6];
		uint32_t hFile_4;
		uint32_t aUn4[6];
		uint32_t uiSlot1_Handle;
		uint32_t uiSlot1_Un0;
		uint32_t uiSlot1_Handle_0;
		uint32_t aUn5[49];
		uint32_t pResSize;
		uint32_t nDecompressSize;
		uint8_t* pResBuffer_Active;
		uint32_t uiEntryCount;
		uint32_t uiIsEntryMaxCount;
		uint32_t uiUnx;
		GSD_FS_Pack_Entry memEntry;
		uint8_t* pResBuffer_Org;
		uint32_t uiUnSize;
	};


	// uiType [1-12]
	// GSD::VM::Parser::TextStruct [004CECF0]
	
	// Set Font Info
	// 05000000 00000000 00000000
	// [OP][ColorSlot][FontSize]

	// DBCS Char
	// 07000000 00000000 00000000
	// [OP][Unknow][DBCSChar]

	// Beg Notation
	// 08000000 00000000 00000000
	// [OP][NotationLenForword][Unknow]

	// End Notation
	// 09000000 00000000 00000000
	// [OP][Unknow][Unknow]

	// End of Struct
	// 0D000000 00000000 00000000
	// [OP][Unknow][Unknow]

	struct SPT_Text_Entry
	{
		uint32_t uiCharCount;
		uint32_t aUn0[2];
		// SPT_Char aText[uiCharCount];
	};
#pragma pack(pop)


	// version 3.0.2.9
	typedef GSD_STD_String_T0* (__thiscall* Fn_GSD_STD_Str_Ctor_T0)(GSD_STD_String_T0* This, char* pStr);
	typedef bool(__thiscall* Fn_FS_LoadFileViaDir_T0)(GSD_FS* This, GSD_STD_String_T0 msFileName, uint32_t uiZLC3Flag);
	typedef bool(__thiscall* Fn_FS_LoadFileViaPack_T0)(GSD_FS* This, GSD_STD_String_T0 msPackName, GSD_STD_String_T0 msFileName, uint8_t uiZLC3Flag);

	// version 3.0.1.8
	typedef GSD_STD_String_T1* (__thiscall* Fn_GSD_STD_Str_Ctor_T1)(GSD_STD_String_T1* This, char* pStr);
	typedef bool(__thiscall* Fn_FS_LoadFileViaDir_T1)(GSD_FS* This, GSD_STD_String_T1 msFileName, uint32_t uiZLC3Flag);
	typedef bool(__thiscall* Fn_FS_LoadFileViaPack_T1)(GSD_FS* This, GSD_STD_String_T1 msPackName, GSD_STD_String_T1 msFileName, uint8_t uiZLC3Flag);
}