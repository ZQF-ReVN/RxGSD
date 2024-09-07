#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <ZxFS/Core.h>
#include <ZxFS/Walker.h>
#include <ZxMem/ZxMem.h>
#include <ZxCvt/ZxCvt.h>
#include <ZxJson/JIO.h>
#include <RxGSD/Core/SPT_File.h>
#include <RxGSD/Core/SPT_Global.h>


static auto Export(const std::vector<std::string>& vcName, const std::string_view msSptPath, const std::string_view msJsonPath, const std::size_t nCodePage) -> bool
{
	ZQF::RxGSD::SPT::File spt{ msSptPath };

	ZQF::ZxCvt cvt;
	ZQF::ZxJson::JArray_t msg_json;
	for (auto& code : spt.GetCodeList())
	{
		if (code.GetArgType0().GetType0TextLen() == 0) { continue; };

		ZQF::ZxJson::JValue msg_obj;

		const auto char_name_seq{ code.GetArgType0().GetNameSeq() };
		if (char_name_seq != static_cast<std::uint32_t>(-1))
		{
			const auto char_name = vcName[char_name_seq];
			msg_obj["chr_raw"] = char_name;
			msg_obj["chr_tra"] = char_name;
		}

		const auto msg_text{ cvt.MBCSToUTF8(code.GetArgType0().GetType0Text(), nCodePage) };
		msg_obj["msg_raw"] = msg_text;
		msg_obj["msg_tra"] = msg_text;

		msg_json.emplace_back(std::move(msg_obj));
	}

	if (msg_json.empty())
	{
		std::println("not find text: -> {}", msSptPath);
		return false;
	}

	ZQF::ZxJson::StoreViaFile(msJsonPath, ZQF::ZxJson::JValue{ std::move(msg_json) }, true, true);

	return true;
}

static auto Import(const std::string_view msSptPath, const std::string_view msJsonPath, const std::string_view msSptNewPath, const std::size_t nCodePage) -> bool
{
	ZQF::RxGSD::SPT::File spt{ msSptPath };

	const auto msg_json_doc{ ZQF::ZxJson::LoadViaFile(msJsonPath) };
	const auto& msg_vec{ msg_json_doc.GetArray() };

	ZQF::ZxCvt cvt;
	std::size_t msg_idx{};
	for (auto& code : spt.GetCodeList())
	{
		if (code.GetArgType0().GetType0TextLen() == 0) { continue; };

		const auto text_u8{ msg_vec.at(msg_idx).At("msg_tra").GetStrView() };
		const auto text_ansi{ cvt.UTF8ToMBCS(text_u8, nCodePage) };
		code.GetArgType0().SetType0Text(text_ansi);
		msg_idx++;
	}

	if (msg_idx == 0)
	{
		std::println("not find text: -> {}", msSptPath);
		return false;
	}

	if (msg_idx != msg_vec.size())
	{
		std::println("text count mismatch: -> {}", msSptPath);
		return false;
	}

	spt.Make().Save(msSptNewPath);

	return true;
}


auto main(void) -> int
{
	try
	{
		ZQF::RxGSD::SPT::Global global;
		global.Load("spt_dec/global.dat");
		[[maybe_unused]] std::vector<std::string> name_list{ global.GetStrTable(932) };

		
		const std::string_view spt_dir{ "spt_dec/" };

		// export batch
		const std::string_view json_dir{ "spt_json/" };
		ZQF::ZxFS::DirMake(json_dir, true);
		for (ZQF::ZxFS::Walker walker{ spt_dir }; walker.NextFile();)
		{
			if (walker.IsSuffix(".spt") == false) { continue; }

			const auto spt_file_path{ walker.GetPath() };
			const auto json_file_name{ std::string{ ZQF::ZxFS::FileNameStem(walker.GetName()) }.append(".json") };
			const auto json_file_path{ std::string{ json_dir }.append(json_file_name) };
			::Export(name_list, spt_file_path, json_file_path, 932);
		}

		// import batch
		const std::string_view spt_new_dir{ "spt_new/" };
		ZQF::ZxFS::DirMake(spt_new_dir, true);
		for (ZQF::ZxFS::Walker walker{ json_dir }; walker.NextFile();)
		{
			if (walker.IsSuffix(".json") == false) { continue; }

			const auto json_file_path{ walker.GetPath() };
			const auto spt_file_name{ std::string{ ZQF::ZxFS::FileNameStem(walker.GetName()) }.append(".spt") };
			const auto spt_file_path{ std::string{ spt_dir }.append(spt_file_name) };
			const auto spt_file_sav_path{ std::string{ spt_new_dir }.append(spt_file_name) };
			::Import(spt_file_path, json_file_path, spt_file_sav_path, 932);
		}
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}
}