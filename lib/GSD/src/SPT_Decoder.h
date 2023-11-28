#pragma once
#include <cstdint>


namespace GSD
{
	class SPT
	{
	private:
		static void DecodeRound0(uint8_t* pData, size_t nSize, size_t nType);
		static void DecodeRound1(uint32_t* pTable, uint8_t* pData, size_t nSize, size_t nStart);

	public:
		static void Decode(uint8_t* pData, size_t nSize);
	};
}