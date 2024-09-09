#pragma once
#include <string>
#include <vector>
#include <ReVN/RxGSD/Core/SPT_Arg.h>


namespace ZQF::ReVN::RxGSD::SPT
{
	class Code
	{
	private:
		std::uint32_t m_uiCommand{};
		std::uint32_t m_uiVal_1{};
		std::uint32_t m_uiVal_2{};
		std::uint32_t m_uiVal_3{};
		std::uint32_t m_uiVal_4{};
		std::uint32_t m_uiSequnece{};
		std::uint32_t m_uiArgType1Count{};
		std::uint32_t m_uiArgType2Count{};
		std::uint32_t m_uiArgType3Count{};

		SPT::Arg_Type0 m_ArgType0;
		std::vector<SPT::Arg_Type1> m_vcArgType1;
		std::vector<SPT::Arg_Type2> m_vcArgType2;
		std::vector<SPT::Arg_Type3> m_vcArgType3;

	public:
		Code();
		Code(ZxMem& vMem);
		Code(const ZxJson::JValue& rfJson, size_t nCodePage);

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto GetCommand() const -> std::size_t;
		auto GetArgType0() -> SPT::Arg_Type0&;

	public:
		auto SizeBytes() const -> std::size_t;
	};
}