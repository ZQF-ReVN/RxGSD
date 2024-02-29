#include <iostream>
#include <format>
#include <filesystem>

#include <GSD/SPT_File.h>
#include <GSD/SPT_Global.h>
#include <GSD/SPT_Cryptor.h>
#include <GSD/GSP_Editor.h>
#include <Rut/RxFile.h>
#include <Rut/RxMem.h>
#include <Rut/RxJson.h>
#include <Rut/RxBench.h>
#include <Rut/RxCmd.h>


static void TestMake()
{
	try
	{
		std::filesystem::create_directory(L"spt_json_meta/");
		std::filesystem::create_directory(L"spt_dump/");

		for (auto& entry : std::filesystem::directory_iterator(L"spt_dec/"))
		{
			if (entry.is_regular_file() == false) { continue; }
			if (entry.path().extension() != L".spt") { continue; }

			const std::filesystem::path& sdt_path = entry.path();
			std::wcout << L"Parser: " << sdt_path.wstring() << L"  ";
			{
				Rut::RxMem::Auto spt{ sdt_path };
				Rut::RxMem::View view = spt.GetView();
				GSD::SPT::File parser;
				parser.Load(view);
				parser.Make().SaveData(L"spt_dump/" + sdt_path.filename().wstring());
				GSD::SPT::CheckDumpDataMatched(spt.GetPtr(), parser);
				auto json = parser.Make(932);
				Rut::RxJson::Parser::Save(json, L"spt_json_meta/" + sdt_path.filename().wstring() + L".json", true, true);
			}
			std::wcout << L"OK" << L'\n';
		}
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

static bool Export(std::vector<std::wstring>& vcName, const std::filesystem::path& phSpt, const std::filesystem::path& phJson, size_t nCodePage)
{
	GSD::SPT::File spt(phSpt);

	// Append Text To Json
	Rut::RxJson::JArray msg_json;
	for (auto& code : spt.GetCodeList())
	{
		if (code.GetArgType0().GetType0TextLen() == 0) { continue; };

		Rut::RxJson::JValue msg_obj;

		size_t char_name_seq = code.GetArgType0().GetNameSeq();
		if (char_name_seq != -1)
		{
			std::wstring_view char_name = vcName[char_name_seq];
			msg_obj[L"chr_raw"] = char_name;
			msg_obj[L"chr_tra"] = char_name;
		}

		std::wstring msg_text = code.GetArgType0().GetType0Text(nCodePage);
		msg_obj[L"msg_raw"] = msg_text;
		msg_obj[L"msg_tra"] = std::move(msg_text);

		msg_json.emplace_back(std::move(msg_obj));
	}

	// Check Msg
	if (msg_json.empty())
	{
		Rut::RxCmd::PutFormat(L"Not Find Text: %s\n", phSpt.wstring().c_str());
		return false;
	}

	// Save Json
	Rut::RxJson::Parser::Save(msg_json, phJson, true);

	return true;
}

static bool Import(std::vector<std::wstring>& vcName, const std::filesystem::path& phSpt, const std::filesystem::path& phJson, const std::filesystem::path& phSptNew, size_t nCodePage)
{
	GSD::SPT::File spt(phSpt);

	// Parse Text Json
	Rut::RxJson::JValue msg_json_doc = Rut::RxJson::Parser{}.Load(phJson);
	Rut::RxJson::JArray& msg_list = msg_json_doc.ToAry();
	Rut::RxJson::JArray::iterator ite_msg_obj = msg_list.begin();

	// Import Text
	for (auto& code : spt.GetCodeList())
	{
		if (code.GetArgType0().GetType0TextLen() == 0) { continue; };

		std::wstring_view text_wide = (*ite_msg_obj)[L"msg_tra"];
		std::string text_ansi = GSD::SPT::Str::MakeANSI(text_wide, nCodePage);
		code.GetArgType0().SetType0Text(text_ansi);
		ite_msg_obj++;
	}

	if (ite_msg_obj == msg_list.begin())
	{
		Rut::RxCmd::PutFormat(L"Not Find Text: %s\n", phSpt.wstring().c_str());
		return false;
	}

	if (ite_msg_obj != msg_list.end())
	{
		Rut::RxCmd::PutFormat(L"Text Count Mismatch: %s\n", phJson.wstring().c_str());
		return false;
	}

	// Save SPT Script
	spt.Make().SaveData(phSptNew);

	return true;
}

static void TestExportText()
{
	GSD::SPT::Global global;
	global.Load(L"global.dat");
	std::vector<std::wstring> name_list = global.GetStrTable(932);

	std::filesystem::path spt_folder = L"spt_dec/";
	std::filesystem::path json_folder = L"spt_txt/";
	std::filesystem::path spt_new_folder = L"spt_new/";
	std::filesystem::create_directories(json_folder);
	//for (auto& entry : std::filesystem::directory_iterator(spt_folder))
	//{
	//	if (entry.is_regular_file() == false) { continue; }
	//	if (entry.path().extension() != L".spt") { continue; }
	//	const std::filesystem::path& spt_path = entry.path();
	//	const std::filesystem::path& json_path = json_folder / entry.path().filename().replace_extension(L".json");
	//	::Export(name_list, spt_path, json_path, 932);
	//}

	std::filesystem::create_directories(spt_new_folder);
	for (auto& entry : std::filesystem::directory_iterator(json_folder))
	{
		if (entry.is_regular_file() == false) { continue; }
		if (entry.path().extension() != L".json") { continue; }
		std::filesystem::path spt_name = entry.path().filename().replace_extension(L".spt");
		const std::filesystem::path& spt_path = spt_folder / spt_name;
		const std::filesystem::path& spt_save_path = spt_new_folder / spt_name;
		::Import(name_list, spt_path, entry, spt_save_path, 932);
	}
}

static void TestHdrParse()
{
	std::cout << "[test]: TestHdrParse\n";

	size_t success_count = 0;
	for (auto& entry : std::filesystem::directory_iterator(L"spt/"))
	{
		if (entry.is_regular_file() == false) { continue; }
		if (entry.path().extension() != L".spt") { continue; }

		Rut::RxMem::Auto spt_org_mem(entry);
		Rut::RxMem::View spt_org_viw = spt_org_mem.GetView();
		size_t spt_real_size = 0;

		GSD::SPT::HDR spt_file1;
		spt_file1.Load(spt_org_viw);
		Rut::RxJson::JValue spt_file1_json = spt_file1.Make(932);

		spt_real_size = spt_file1.GetSize();

		GSD::SPT::HDR spt_file2;
		spt_file2.Load(spt_file1_json, 932);

		Rut::RxMem::Auto spt_file1_mem = spt_file1.Make();
		Rut::RxMem::Auto spt_file2_mem = spt_file2.Make();

		GSD::SPT::HDR spt_file3;
		Rut::RxMem::View spt_file1_view = spt_file1_mem.GetView();
		spt_file3.Load(spt_file1_view);

		// Check Reload Data
		if (spt_file1_mem.GetSize() == spt_file2_mem.GetSize())
		{
			if (memcmp(spt_file1_mem.GetPtr(), spt_file2_mem.GetPtr(), spt_file1_mem.GetSize()) != 0)
			{
				Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
				continue;
			}
		}
		else
		{
			Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
			continue;
		}

		// Check Org Data
		if (spt_file1_mem.GetSize() == spt_real_size)
		{
			if (memcmp(spt_file1_mem.GetPtr(), spt_org_mem.GetPtr(), spt_real_size) != 0)
			{
				Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
				continue;
			}
		}
		else
		{
			Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
			continue;
		}

		std::cout << entry << std::endl;
		success_count += 1;
	}

	Rut::RxCmd::PutFormat(L"SUCCESS:%d\n", success_count);
}

static void TestSptParse()
{
	std::cout << "[test]: TestSptParse\n";

	size_t success_count = 0;
	for (auto& entry : std::filesystem::directory_iterator(L"spt/"))
	{
		if (entry.is_regular_file() == false) { continue; }
		if (entry.path().extension() != L".spt") { continue; }

		Rut::RxMem::Auto spt_org_mem(entry);
		Rut::RxMem::View spt_org_viw = spt_org_mem.GetView();
		size_t spt_real_size = 0;

		GSD::SPT::File spt_file1;
		spt_file1.Load(spt_org_viw);
		Rut::RxJson::JValue spt_file1_json = spt_file1.Make(932);

		spt_real_size = spt_file1.GetSize();

		GSD::SPT::File spt_file2;
		spt_file2.Load(spt_file1_json, 932);

		Rut::RxMem::Auto spt_file1_mem = spt_file1.Make();
		Rut::RxMem::Auto spt_file2_mem = spt_file2.Make();

		GSD::SPT::File spt_file3;
		Rut::RxMem::View spt_file1_view = spt_file1_mem.GetView();
		spt_file3.Load(spt_file1_view);

		// Check Reload Data
		if (spt_file1_mem.GetSize() == spt_file2_mem.GetSize())
		{
			if (memcmp(spt_file1_mem.GetPtr(), spt_file2_mem.GetPtr(), spt_file1_mem.GetSize()) != 0)
			{
				Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
				continue;
			}
		}
		else
		{
			Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
			continue;
		}

		// Check Org Data
		if (spt_file1_mem.GetSize() == spt_real_size)
		{
			if (memcmp(spt_file1_mem.GetPtr(), spt_org_mem.GetPtr(), spt_real_size) != 0)
			{
				Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
				continue;
			}
		}
		else
		{
			Rut::RxCmd::PutFormat(L"FAILED:%s\n", entry.path().wstring().data());
			continue;
		}

		std::cout << entry << std::endl;
		success_count += 1;
	}

	Rut::RxCmd::PutFormat(L"SUCCESS:%d\n", success_count);
}

static void TestSptDecrypt()
{
	std::filesystem::path save_path = L"spt_dec";
	for (auto& entry : std::filesystem::directory_iterator("spt"))
	{
		if (entry.is_regular_file() == false) { continue; }
		const std::filesystem::path& spt_path = entry.path();
		Rut::RxMem::Auto spt_file{ spt_path.wstring() };
		GSD::SPT::Cryptor::Decode(spt_file, true);
		spt_file.SaveData(save_path / spt_path.filename());
	}
}

static void TestGSP()
{
	GSD::GSP::Pack(L"data/", L"data1.gsp");
}

static void TestBMZ()
{

}

int main()
{
	TestHdrParse();
	TestSptParse();
}