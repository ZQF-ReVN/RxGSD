#pragma once
#include <string>
#include <vector>
#include <ZxMem/ZxMem.h>
#include <ZxJson/JValue.h>
#include <RxGSD/Core/GSD_Struct.h>


namespace ZQF::RxGSD::SPT
{
	class Arg_Type0
	{
	private:
		std::uint32_t m_uiNameReallySeq{};
		std::uint32_t m_uiNameDisplaySeq{};
		std::uint32_t m_uiUn2{};
		std::uint32_t m_uiVoiceFileSeq{};
		std::uint32_t m_uiStrType0Len{}; // dbcs char count
		std::uint32_t m_uiStrType1Len{};
		std::uint32_t m_uiStrType2Len{};

		std::vector<Struct::SPT_Char_Entry> m_vcStrType0;
		std::string m_msStrType1;
		std::string m_msStrType2;

	public:
		Arg_Type0();
		Arg_Type0(ZxMem& rfMem);
		Arg_Type0(const ZxJson::JValue& rfJson, const std::size_t nCodePage);

	public:
		auto Load(ZxMem& rfMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& rfMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SetType0Text(const std::string_view msText) -> void;
		auto SetType1Text(const std::string_view msText) -> void;
		auto SetType2Text(const std::string_view msText) -> void;

	public:
		auto GetType0TextLen() const -> std::size_t;
		auto GetType0Text() const -> std::string;
		auto GetNameSeq() const -> std::size_t;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class Arg_Type1
	{
	private:
		std::uint32_t m_uiVal_0{};
		std::uint32_t m_uiVal_1{};
		std::uint32_t m_uiVal_2{};
		std::uint32_t m_uiVal_3{};
		std::uint32_t m_uiStrLen{};
		std::uint32_t m_uiVal_5{};
		std::uint8_t m_ucVal_6{};
		std::uint32_t m_uiVal_7{};
		std::string m_msStr;

	public:
		Arg_Type1();
		Arg_Type1(ZxMem& vMem);
		Arg_Type1(const ZxJson::JValue& rfJson, const std::size_t nCodePage);

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SetStr(const std::string_view msStr) -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class Arg_Type2
	{
	private:
		std::uint32_t m_uiParameterType1Count{};
		std::vector<Arg_Type1> m_vcParameterType1;

	public:
		Arg_Type2();
		Arg_Type2(ZxMem& vMem);
		Arg_Type2(const ZxJson::JValue& rfJson, const std::size_t nCodePage);

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;

	};

	class Arg_Type3
	{
	private:
		std::uint32_t m_uiVal_0{};
		std::uint32_t m_uiVal_1{};
		std::uint32_t m_uiVal_2{};

	public:
		Arg_Type3();
		Arg_Type3(ZxMem& vMem);
		Arg_Type3(const ZxJson::JValue& rfJson, const std::size_t nCodePage);

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SizeBytes() const->std::size_t;
	};
}