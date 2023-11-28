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

	//struct GSP_HDR
	//{
	//	uint32_t uiEntryCount;
	//	GSP_Entry aEntry[uiEntryCount];
	//};

	struct BMZ
	{
		uint8_t aSignature[4]; // ZLC3
		uint32_t uiBMPSize;
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

	typedef GSD_STD_String* (__thiscall* Fn_GSD_STD_Str_Ctor_T0)(GSD_STD_String* This, char* pStr);
	typedef bool (__thiscall* Fn_FS_LoadFileViaDir)(GSD_FS* This, GSD_STD_String msFileName, size_t uiStrPx, size_t uiZLC3Flag);
	typedef bool (__thiscall* Fn_FS_LoadFileViaPack)(GSD_FS* This, GSD_STD_String msPackName, size_t uiStrAp0, GSD_STD_String msFileName, size_t uiStrAp1, size_t uiZLC3Flag);
}