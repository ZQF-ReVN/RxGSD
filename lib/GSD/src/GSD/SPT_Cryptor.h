#pragma once
#include <span>


namespace GSD::SPT
{
	class Cryptor
	{
	private:
		static void DecodeRound0(std::span<uint8_t> spEnc, size_t nType);
		static void DecodeRound1(uint32_t* pTable, std::span<uint8_t> spEnc, size_t nStart);

	public:
		static void Decode(std::span<uint8_t> spSpt, bool isMakeReadable);
	};
}