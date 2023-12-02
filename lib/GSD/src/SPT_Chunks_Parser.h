#pragma once
#include <cstdint>
#include <vector>
#include <iostream>


namespace GSD::SPT
{
	// if str_len == 0 {size = 0x1D} else {size = 0x1D + strlen + 1}
	class SPT_Code_Append_Data_Type1_Ele
	{
	public:
		uint32_t m_uiVal_0;
		uint32_t m_uiVal_1;
		uint32_t m_uiVal_2;
		uint32_t m_uiVal_3;
		uint32_t m_uiStrLen; // Val_4
		uint32_t m_uiVal_5;
		uint8_t m_uiVal_6;
		uint32_t m_uiVal_7;
		// char aStr[uiStrLen + 1];
		std::string m_msStr;

		SPT_Code_Append_Data_Type1_Ele()
		{
			this->m_uiVal_0 = 0;
			this->m_uiVal_1 = 0;
			this->m_uiVal_2 = 0;
			this->m_uiVal_3 = 0;
			this->m_uiStrLen = 0;
			this->m_uiVal_5 = 0;
			this->m_uiVal_6 = 0;
			this->m_uiVal_7 = 0;
		}

		void Init(uint8_t* pData)
		{
			uint32_t* data_ptr = (uint32_t*)pData;

			this->m_uiVal_0 = data_ptr[0];
			this->m_uiVal_1 = data_ptr[1];
			this->m_uiVal_2 = data_ptr[2];
			this->m_uiVal_3 = data_ptr[3];
			this->m_uiStrLen = data_ptr[4];
			this->m_uiVal_5 = data_ptr[5];

			this->m_uiVal_6 = *((pData + 6 * 4) + 0);
			this->m_uiVal_7 = *(uint32_t*)((pData + 6 * 4) + 1);

			if (this->m_uiStrLen)
			{
				char* str_ptr = (char*)((pData + 6 * 4) + 5);
				this->m_msStr = { str_ptr,this->m_uiStrLen };
			}
		}

		size_t GetSize()
		{
			if (this->m_uiStrLen)
			{
				return 0x1D + this->m_uiStrLen + 1;
			}
			else
			{
				return 0x1D;
			}
		}
	};

	class SPT_Code_Append_Data_Type2_Ele
	{
	public:
		uint32_t m_uiType1Count;
		std::vector<SPT_Code_Append_Data_Type1_Ele> m_vcAppendDataType1Ele;

		SPT_Code_Append_Data_Type2_Ele()
		{
			this->m_uiType1Count = 0;
		}

		void Init(uint8_t* pData)
		{
			this->m_uiType1Count = *(uint32_t*)pData;
			uint8_t* cur_ptr = pData + 4;

			for (size_t ite_type1 = 0; ite_type1 < this->m_uiType1Count; ite_type1++)
			{
				SPT_Code_Append_Data_Type1_Ele type1;
				type1.Init(cur_ptr);
				cur_ptr += type1.GetSize();
				m_vcAppendDataType1Ele.emplace_back(std::move(type1));
			}
		}

		size_t GetSize()
		{
			size_t size = 4;
			for (auto& type1 : m_vcAppendDataType1Ele)
			{
				size += type1.GetSize();
			}
			return size;
		}

	};

	class SPT_Code_Append_Data_Type3_Ele
	{
	public:
		uint32_t m_uiVal_0;
		uint32_t m_uiVal_1;
		uint32_t m_uiVal_2;

		SPT_Code_Append_Data_Type3_Ele()
		{
			this->m_uiVal_0 = 0;
			this->m_uiVal_1 = 0;
			this->m_uiVal_2 = 0;
		}

		void Init(uint8_t* pData)
		{
			uint32_t* data_ptr = (uint32_t*)pData;
			this->m_uiVal_0 = data_ptr[0];
			this->m_uiVal_1 = data_ptr[1];
			this->m_uiVal_2 = data_ptr[2];
		}

		size_t GetSize()
		{
			return 3 * 4;
		}
	};

	struct SPT_Char_Entry
	{
		uint32_t uiType; // [0x7](normal char flag), [0xD](end string flag), [0x8](notation beg flag), [0x9](notation end flag)
		uint32_t uiNotationCount; // 0x0
		uint32_t uiChar; // char
	};

	class SPT_Code_Append_Dara_Type0
	{
	public:
		uint32_t m_uiUn0;
		uint32_t m_uiUn1;
		uint32_t m_uiUn2;
		uint32_t m_uiUn3;
		uint32_t m_uiCharCount;
		uint32_t m_uiStrType0Len;
		uint32_t m_uiStrType1Len;

		std::vector<SPT_Char_Entry> m_vcCharList;
		std::string m_msStrType0;
		std::string m_msStrType1;

		SPT_Code_Append_Dara_Type0()
		{
			this->m_uiUn0 = 0;
			this->m_uiUn1 = 0;
			this->m_uiUn2 = 0;
			this->m_uiUn3 = 0;
			this->m_uiCharCount = 0;
			this->m_uiStrType0Len = 0;
			this->m_uiStrType1Len = 0;
		}

		void Init(uint8_t* pData)
		{
			uint32_t* data_ptr = (uint32_t*)pData;
			this->m_uiUn0 = data_ptr[0];
			this->m_uiUn1 = data_ptr[1];
			this->m_uiUn2 = data_ptr[2];
			this->m_uiUn3 = data_ptr[3];
			this->m_uiCharCount = data_ptr[4];
			this->m_uiStrType0Len = data_ptr[5];
			this->m_uiStrType1Len = data_ptr[6];

			uint8_t* cur_prt = ((uint8_t*)data_ptr) + 4 * 7;

			if (this->m_uiCharCount)
			{
				const SPT_Char_Entry* char_entry_arry = (SPT_Char_Entry*)cur_prt;
				for (size_t ite_entry = 0; ite_entry < this->m_uiCharCount; ite_entry++)
				{
					SPT_Char_Entry entry = char_entry_arry[ite_entry];
					m_vcCharList.push_back(entry);
				}

				cur_prt += m_vcCharList.size() * sizeof(SPT_Char_Entry);
			}
			if (this->m_uiStrType0Len)
			{
				this->m_msStrType0 = (char*)cur_prt;
				cur_prt += this->m_uiStrType0Len + 1;
			}
			if (this->m_uiStrType1Len)
			{
				this->m_msStrType1 = (char*)cur_prt;
				cur_prt += this->m_uiStrType0Len + 1;
			}
		}

		size_t GetSize()
		{
			size_t size = 7 * 4 + m_vcCharList.size() * sizeof(SPT_Char_Entry);
			if (this->m_uiStrType0Len)
			{
				size += this->m_uiStrType0Len + 1;
			}
			if (this->m_uiStrType1Len)
			{
				size += this->m_uiStrType1Len + 1;
			}
			return size;
		}
	};



	class SPT_Code
	{
	public:
		uint32_t m_uiCommand;
		uint32_t m_uiVal_1;
		uint32_t m_uiVal_2;
		uint32_t m_uiVal_3;
		uint32_t m_uiVal_4;
		uint32_t m_uiSequnece;
		uint32_t m_uiAppendDataCount_Type1;
		uint32_t m_uiAppendDataCount_Type2;
		uint32_t m_uiAppendDataCount_Type3;
		// SPT_Code_Append_Data_TypeX[uiAppendDataCount_TypeX]

		SPT_Code_Append_Dara_Type0 m_AppendDataType0;
		std::vector<SPT_Code_Append_Data_Type1_Ele> m_vcAppendDataType1;
		std::vector<SPT_Code_Append_Data_Type2_Ele> m_vcAppendDataType2;
		std::vector<SPT_Code_Append_Data_Type3_Ele> m_vcAppendDataType3;

		SPT_Code()
		{
			this->m_uiCommand = 0;
			this->m_uiVal_1 = 0;
			this->m_uiVal_2 = 0;
			this->m_uiVal_3 = 0;
			this->m_uiVal_4 = 0;
			this->m_uiSequnece = 0;
			this->m_uiAppendDataCount_Type1 = 0;
			this->m_uiAppendDataCount_Type2 = 0;
			this->m_uiAppendDataCount_Type3 = 0;
		}

		void Init(uint8_t* pData)
		{
			uint32_t* data_ptr = (uint32_t*)pData;

			this->m_uiCommand = data_ptr[0];
			this->m_uiVal_1 = data_ptr[1];
			this->m_uiVal_2 = data_ptr[2];
			this->m_uiVal_3 = data_ptr[3];
			this->m_uiVal_4 = data_ptr[4];
			this->m_uiSequnece = data_ptr[5];
			this->m_uiAppendDataCount_Type1 = data_ptr[6];
			this->m_uiAppendDataCount_Type2 = data_ptr[7];
			this->m_uiAppendDataCount_Type3 = data_ptr[8];

			uint8_t* append_data_ptr = pData + 9 * 4;
			if (m_uiCommand == 1) // Proc Text Struct
			{
				m_AppendDataType0.Init(append_data_ptr);
				append_data_ptr += m_AppendDataType0.GetSize();
			}

			for (size_t ite_type1 = 0; ite_type1 < this->m_uiAppendDataCount_Type1; ite_type1++)
			{
				SPT_Code_Append_Data_Type1_Ele type1;
				type1.Init(append_data_ptr);
				append_data_ptr += type1.GetSize();
				m_vcAppendDataType1.push_back(type1);
			}

			for (size_t ite_type2 = 0; ite_type2 < this->m_uiAppendDataCount_Type2; ite_type2++)
			{
				SPT_Code_Append_Data_Type2_Ele type2;
				type2.Init(append_data_ptr);
				append_data_ptr += type2.GetSize();
				m_vcAppendDataType2.push_back(type2);
			}

			for (size_t ite_type3 = 0; ite_type3 < this->m_uiAppendDataCount_Type3; ite_type3++)
			{
				SPT_Code_Append_Data_Type3_Ele type3;
				type3.Init(append_data_ptr);
				append_data_ptr += type3.GetSize();
				m_vcAppendDataType3.push_back(type3);
			}
		}

		size_t GetSize()
		{
			size_t code_size = 9 * 4;

			if (this->m_uiCommand == 1)
			{
				code_size += m_AppendDataType0.GetSize();
			}

			for (auto& type1 : this->m_vcAppendDataType1)
			{
				code_size += type1.GetSize();
			}

			for (auto& type2 : this->m_vcAppendDataType2)
			{
				code_size += type2.GetSize();
			}

			for (auto& type3 : this->m_vcAppendDataType3)
			{
				code_size += type3.GetSize();
			}

			return code_size;
		}

	};

	class SPT_Chunks_Info
	{
	public:
		uint32_t m_uiChunkCount;
		uint32_t m_uiUn0;
		uint32_t m_uiUn1;
		uint32_t m_uiUn2;

		SPT_Chunks_Info()
		{
			this->m_uiChunkCount = 0;
			this->m_uiUn0 = 0;
			this->m_uiUn1 = 0;
			this->m_uiUn2 = 0;
		}

		void Init(uint8_t* pInfo)
		{
			uint32_t* info_chunk_ptr = (uint32_t*)pInfo;

			this->m_uiChunkCount = info_chunk_ptr[0];
			this->m_uiUn0 = info_chunk_ptr[1];
			this->m_uiUn1 = info_chunk_ptr[2];
			this->m_uiUn2 = info_chunk_ptr[3];
		}

		size_t GetSize()
		{
			return 4 * 4;
		}
	};

	class SPT_Append_Script_info
	{
	public:
		uint32_t m_uiStrLen0;
		//char aScriptName[uiStrLen0];
		std::string m_msStr0;
		uint32_t m_uiStrLen1;
		//char aScriptName[uiStrLen1];
		std::string m_msStr1;
		uint32_t m_uiUn0;
		uint32_t m_uiUn1;
		uint32_t m_uiUn2;
		char m_aAppend[0x80];

		SPT_Append_Script_info() : m_aAppend{}
		{
			this->m_uiStrLen0 = 0;
			this->m_uiStrLen1 = 0;
			this->m_uiUn0 = 0;
			this->m_uiUn1 = 0;
			this->m_uiUn2 = 0;

		}

		void Init(uint8_t* pData)
		{
			uint8_t* cur_ptr = pData;

			this->m_uiStrLen0 = *(uint32_t*)cur_ptr;
			if (this->m_uiStrLen0)
			{
				char* str0_ptr = (char*)(cur_ptr + 4);
				this->m_msStr0 = { str0_ptr , this->m_uiStrLen0 };
			}
			cur_ptr += 4 + this->m_uiStrLen0;

			this->m_uiStrLen1 = *(uint32_t*)cur_ptr;
			if (this->m_uiStrLen1)
			{
				char* str1_ptr = (char*)(cur_ptr + 4);
				this->m_msStr1 = { str1_ptr , this->m_uiStrLen1 };
			}
			cur_ptr += 4 + this->m_uiStrLen1;

			this->m_uiUn0 = ((uint32_t*)cur_ptr)[0];
			this->m_uiUn1 = ((uint32_t*)cur_ptr)[1];
			this->m_uiUn2 = ((uint32_t*)cur_ptr)[2];
		}

		size_t GetSize()
		{
			return 4 * 2 + 0x80 + m_msStr0.size() + m_msStr1.size() + 4 * 3;
		}
	};

	// if uiScriptCount == 0 {size = 4};
	class SPT_Append_Script
	{
	public:
		uint32_t m_uiScriptCount;
		std::vector<SPT_Append_Script_info> m_vcInfo;

		SPT_Append_Script()
		{
			this->m_uiScriptCount = 0;
		}

		void Init(uint8_t* pData)
		{
			this->m_uiScriptCount = *(uint32_t*)pData;

			uint8_t* cur_prt = pData + 4;

			for (size_t ite_info = 0; ite_info < this->m_uiScriptCount; ite_info++)
			{
				SPT_Append_Script_info info;
				info.Init(cur_prt);
				cur_prt += info.GetSize();
				m_vcInfo.emplace_back(std::move(info));
			}
		}

		size_t GetSize()
		{
			size_t size = 4;
			for (auto& info : this->m_vcInfo)
			{
				size += info.GetSize();
			}
			return size;
		}
	};


	class SPT_HDR
	{
	public:
		uint8_t m_ucDecStartIndex;
		uint8_t m_ucDecModeType;
		uint8_t m_ucUn0;
		uint8_t m_ucUn1;
		uint32_t m_uiUn2; // == 0
		//uint32_t m_uiScriptNameLen;
		std::string m_msScriptName;
		SPT_Chunks_Info m_ChunksInfo;
		SPT_Append_Script m_aAppendScript[0xF];
		uint32_t m_uiUnsize;

		SPT_HDR()
		{
			this->m_ucDecStartIndex = 0;
			this->m_ucDecModeType = 0;
			this->m_ucUn0 = 0;
			this->m_ucUn1 = 0;
			this->m_uiUn2 = 0;
			this->m_uiUnsize = 0;
		}

		void Init(uint8_t* pHdr)
		{
			this->m_ucDecStartIndex = pHdr[0];
			this->m_ucDecModeType = pHdr[1];
			this->m_ucUn0 = pHdr[2];
			this->m_ucUn1 = pHdr[3];
			this->m_uiUn2 = *((uint32_t*)(pHdr + (4 * 1)) + 0);
			uint32_t script_name_len = *((uint32_t*)(pHdr + (4 * 1)) + 1);
			char* script_name_ptr = (char*)((uint32_t*)(pHdr + (4 * 1)) + 2);
			this->m_msScriptName = { script_name_ptr, script_name_len };
			uint8_t* chunks_info_ptr = ((uint8_t*)script_name_ptr + script_name_len);
			this->m_ChunksInfo.Init(chunks_info_ptr);

			uint8_t* append_script_ptr = pHdr + (4 + 4 + 4 + script_name_len + this->m_ChunksInfo.GetSize());
			for (auto& append_script : this->m_aAppendScript)
			{
				append_script.Init(append_script_ptr);
				append_script_ptr += append_script.GetSize();
			}

			this->m_uiUnsize = *(uint32_t*)append_script_ptr;
		}

		size_t GetSize()
		{
			size_t size = 4 * 2 + 4 + m_msScriptName.size() + m_ChunksInfo.GetSize();
			for (auto& append_script : this->m_aAppendScript)
			{
				size += append_script.GetSize();
			}
			size += 4;
			return size;
		}
	};

	class SPT_Parser
	{
	public:
		SPT_HDR m_HDR;
		std::vector<SPT_Code> m_vcCode;

		SPT_Parser()
		{

		}

		void Init(uint8_t* pSPT)
		{
			uint8_t* cur_ptr = pSPT;

			this->m_HDR.Init(cur_ptr);
			cur_ptr += this->m_HDR.GetSize();

			for (size_t ite_chunk = 0; ite_chunk < this->m_HDR.m_ChunksInfo.m_uiChunkCount; ite_chunk++)
			{
				SPT_Code code;
				code.Init(cur_ptr);
				cur_ptr += code.GetSize();
				m_vcCode.emplace_back(std::move(code));
			}
		}
	};
}