#pragma once
#include <string>
#include <vector>
#include <ZxMem/ZxMem.h>
#include <ZxJson/JValue.h>
#include <RxGSD/Core/GSD_Struct.h>


namespace ZQF::RxGSD::SPT
{
	class Append_Script_Entry
	{
	private:
		std::uint32_t m_uiStrLen0{};
		std::string m_msStr0;
		std::uint32_t m_uiStrLen1{};
		std::string m_msStr1;
		std::uint32_t m_uiUn0{};
		std::uint32_t m_uiUn1{};
		std::uint32_t m_uiUn2{};
		char m_aAppend[0x80]{}; // Maybe there's no data in here.

	public:
		Append_Script_Entry();
		Append_Script_Entry(ZxMem& vMem);
		Append_Script_Entry(const ZxJson::JValue& rfJson, const std::size_t nCodePage);

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class Append_Script
	{
	private:
		std::uint32_t m_uiScriptCount{};
		std::vector<Append_Script_Entry> m_vcInfo;

	public:
		Append_Script();

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class Encryptor_Info
	{
	private:
		std::uint8_t m_ucDecStartIndex{};
		std::uint8_t m_ucDecModeType{};
		std::uint8_t m_ucUn0{};
		std::uint8_t m_ucUn1{};

	public:
		Encryptor_Info();

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class Codes_Info
	{
	private:
		std::uint32_t m_uiCodeCount{};
		std::uint32_t m_uiUn0{};
		std::uint32_t m_uiUn1{};
		std::uint32_t m_uiUn2{};

	public:
		Codes_Info();

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make(ZxMem& amMem) const -> void;
		auto Make(ZxJson::JValue& rfJson, const std::size_t nCodePage) const -> void;

	public:
		auto GetCodeCount() const -> std::size_t;

	public:
		auto SizeBytes() const -> std::size_t;
	};

	class HDR
	{
	private:
		Encryptor_Info m_EncryptorInfo;
		std::uint32_t m_uiUnCount{};
		std::uint32_t m_uiScriptNameLen{};
		std::string m_msScriptName;
		Codes_Info m_CodesInfo;
		Append_Script m_aAppendScript[0xF];
		std::uint32_t m_uiUnsize{};

	public:
		HDR();

	public:
		auto Load(ZxMem& vMem) -> void;
		auto Load(const std::string_view msSpt) -> void;
		auto Load(const ZxJson::JValue& rfJson, const std::size_t nCodePage) -> void;
		auto Make() const -> ZxMem;
		auto Make(const std::size_t nCodePage) const -> ZxJson::JValue;

	public:
		auto GetCodeCount() const -> std::size_t;

	public:
		auto SizeBytes() const -> std::size_t;
	};
}