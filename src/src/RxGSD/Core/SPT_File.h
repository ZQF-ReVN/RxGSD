#pragma once
#include <vector>
#include <string_view>
#include <Zut/ZxMem.h>
#include <Zut/ZxJson.h>
#include <RxGSD/Core/SPT_HDR.h>
#include <RxGSD/Core/SPT_Code.h>


namespace ZQF::RxGSD::SPT
{
	class File
	{
	private:
		HDR m_HDR;
		std::vector<SPT::Code> m_vcCode;

	public:
		File();
		File(const std::string_view msSptPath);
		File(const ZxJson::JValue& rfJson, const std::size_t nCodePage);

	public:
		auto Load(const std::string_view msSptPath) -> void;
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make() const -> ZxMem;
		auto Make(const std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetCodeList() -> std::vector<SPT::Code>&;

	public:
		auto SizeBytes() const -> std::size_t;
	};
}