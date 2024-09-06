#pragma once
#include <span>
#include <cstdint>


namespace ZQF::RxGSD::SPT
{
	class Cryptor
	{
	private:
		static auto DecodeRound0(const std::span<std::uint8_t> spEnc, const std::size_t nType) -> void;
		static auto DecodeRound1(const std::uint32_t* pTable, const std::span<std::uint8_t> spEnc, const std::size_t nStart) -> void;

	public:
		static auto Decode(const std::span<std::uint8_t> spSpt, const bool isMakeReadable) -> void;
	};
}