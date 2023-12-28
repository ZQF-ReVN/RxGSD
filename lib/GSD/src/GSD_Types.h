#pragma once
#include <cstdint>
#include <Windows.h>


namespace GSD
{
	struct GSP_Entry
	{
		uint32_t uiFOA;
		uint32_t uiSize;
		char aFileName[0x38];
	};

	//struct GSP_File
	//{
	//	uint32_t uiEntryCount;
	//	GSP_Entry aEntry[uiEntryCount];
	//  // data..
	//};

	struct GSD_BMZ_HDR
	{
		uint8_t aSignature[4]; // ZLC3
		uint32_t uiDecompressSize; // BMP
		// zlib compress data
	};

	struct GSD_STD_String
	{
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

	struct SPT_Char_Entry
	{
		uint32_t uiType; // [0x7](normal char flag), [0xD](end string flag), [0x8](notation beg flag), [0x9](notation end flag)
		uint32_t uiParam0;
		uint32_t uiParan1;
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


	typedef GSD_STD_String* (__thiscall* Fn_GSD_STD_Str_Ctor_T0)(GSD_STD_String* This, char* pStr);
	typedef bool(__thiscall* Fn_FS_LoadFileViaDir)(GSD_FS* This, GSD_STD_String msFileName, uint32_t uiStrPx, uint32_t uiZLC3Flag);

	// version 3.0.2.9
	typedef bool(__thiscall* Fn_FS_LoadFileViaPack_Type0)(GSD_FS* This, GSD_STD_String msPackName, uint32_t uiStrAp0, GSD_STD_String msFileName, uint32_t uiStrAp1, uint32_t uiZLC3Flag);

	// version 3.0.1.8
	typedef bool(__thiscall* Fn_FS_LoadFileViaPack_Type1)(GSD_FS* This, uint32_t uiStrAp0, GSD_STD_String msPackName, uint32_t uiStrAp1, GSD_STD_String msFileName, uint32_t uiZLC3Flag);
}