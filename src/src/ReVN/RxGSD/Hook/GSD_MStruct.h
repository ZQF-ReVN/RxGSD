#pragma once
#include <cstdint>


namespace ZQF::ReVN::RxGSD
{
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
}


namespace ZQF::ReVN::RxGSD
{
#pragma pack(push,1)
	struct MStruct
	{
		// version 3.0.1.8
		struct Ver3018
		{
			struct STD_String
			{
			public:
				typedef auto (__thiscall* Fn_Assign_t)(STD_String* This, const char* cpStr)->STD_String*;
				static inline Fn_Assign_t FnAssign;

			private:
				std::uint32_t m_nAlig{};
				union
				{
					char* m_cpStr{};
					char m_aStr[16];
				};
				std::uint32_t m_nBytes{};
				std::uint32_t m_nCapacity{};

			public:
				STD_String() {};

				auto C_Str() const -> const char*
				{
					return this->Size() >= 16 ? this->m_cpStr : this->m_aStr;
				}

				auto Size() const -> std::uint32_t
				{
					return this->m_nBytes;
				}

				auto Assign(const char* cpStr) -> STD_String&
				{
					return *STD_String::FnAssign(this, cpStr);
				}
			};

			struct VFS
			{
			public:
				typedef auto(__thiscall* Fn_LoadFileViaDir_t)(VFS* This, STD_String msFileName, std::uint32_t ucZLC3Flag) -> bool;
				typedef auto(__thiscall* Fn_LoadFileViaPack_t)(VFS* This, STD_String msPackName, STD_String msFileName, std::uint8_t ucZLC3Flag) -> bool;

				struct Pack_Entry
				{
					std::uint64_t uiFOA;
					std::uint64_t uiSize;
					char aFileName[112];
				};

			public:
				void* hFile;
				std::uint32_t uiFilePointer_Back;
				std::uint32_t hFile_0;
				std::uint64_t sOldFilePointer;
				std::uint32_t aUn0[3];
				std::uint64_t sFilePointer;
				std::uint64_t sFileSize;
				std::uint32_t hFile_1;
				std::uint32_t aUn1[28];
				std::uint32_t hFile_2;
				std::uint32_t aUn2[6];
				std::uint32_t hFile_3;
				std::uint32_t aUn3[6];
				std::uint32_t hFile_4;
				std::uint32_t aUn4[6];
				std::uint32_t uiSlot1_Handle;
				std::uint32_t uiSlot1_Un0;
				std::uint32_t uiSlot1_Handle_0;
				std::uint32_t aUn5[49];
				std::uint32_t pResSize;
				std::uint32_t nDecompressSize;
				std::uint8_t* pResBuffer_Active;
				std::uint32_t uiEntryCount;
				std::uint32_t uiIsEntryMaxCount;
				std::uint32_t uiUnx;
				VFS::Pack_Entry memEntry;
				std::uint8_t* pResBuffer_Org;
				std::uint32_t uiUnSize;
			};
		};

		// version 3.0.2.9
		struct Ver3029
		{
			struct STD_String
			{
			public:
				typedef auto (__thiscall* Fn_Assign_t)(STD_String* This, const char* pStr)->STD_String*;
				static inline Fn_Assign_t FnAssign;

			private:
				union
				{
					char* m_cpStr{};
					char m_aStr[16];
				};
				std::uint32_t m_nBytes{};
				std::uint32_t m_nCapacity{};
				std::uint32_t m_nAlig{};

			public:
				STD_String() {};

				auto C_Str() const -> const char*
				{
					return this->Size() >= 16 ? this->m_cpStr : this->m_aStr;
				}

				auto Size() const -> std::uint32_t
				{
					return this->m_nBytes;
				}

				auto Assign(const char* cpStr) -> STD_String&
				{
					return *STD_String::FnAssign(this, cpStr);
				}
			};

			struct VFS
			{
			public:
				typedef auto(__thiscall* Fn_LoadFileViaDir_t)(VFS* This, STD_String msFileName, std::uint32_t ucZLC3Flag) -> bool;
				typedef auto(__thiscall* Fn_LoadFileViaPack_t)(VFS* This, STD_String msPackName, STD_String msFileName, std::uint8_t ucZLC3Flag) -> bool;

				struct Pack_Entry
				{
					std::uint64_t uiFOA;
					std::uint64_t uiSize;
					char aFileName[112];
				};
				
			public:
				void* hFile;
				std::uint32_t uiFilePointer_Back;
				std::uint32_t hFile_0;
				std::uint64_t sOldFilePointer;
				std::uint32_t aUn0[3];
				std::uint64_t sFilePointer;
				std::uint64_t sFileSize;
				std::uint32_t hFile_1;
				std::uint32_t aUn1[28];
				std::uint32_t hFile_2;
				std::uint32_t aUn2[6];
				std::uint32_t hFile_3;
				std::uint32_t aUn3[6];
				std::uint32_t hFile_4;
				std::uint32_t aUn4[6];
				std::uint32_t uiSlot1_Handle;
				std::uint32_t uiSlot1_Un0;
				std::uint32_t uiSlot1_Handle_0;
				std::uint32_t aUn5[49];
				std::uint32_t pResSize;
				std::uint32_t nDecompressSize;
				std::uint8_t* pResBuffer_Active;
				std::uint32_t uiEntryCount;
				std::uint32_t uiIsEntryMaxCount;
				std::uint32_t uiUnx;
				VFS::Pack_Entry memEntry;
				std::uint8_t* pResBuffer_Org;
				std::uint32_t uiUnSize;
			};
		};
	};

#pragma pack(pop)

}
