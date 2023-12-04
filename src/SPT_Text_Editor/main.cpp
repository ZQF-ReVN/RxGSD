#include <iostream>

#include "../../lib/GSD/SPT.h"


void ExportText(const std::wstring& wsPath, size_t nCodePage)
{
	Rut::RxJson::Value json;
	{
		GSD::SPT::Parser parser;
		parser.Parse(wsPath);
		for (auto& code : parser.GetCodeList())
		{
			auto& type0 = code.GetParamType0();
			if (type0.GetType0TextLen())
			{
				std::string text = type0.GetType0Text();
				json.Append(Rut::RxStr::ToWCS(text, nCodePage));
			}
		}
	}
	Rut::RxJson::Parser::Save(json, wsPath + L".json", true);
}

void ImportText(const std::wstring& wsScriptPath, const std::wstring_view wsJsonPath, size_t nCodePage)
{
	GSD::SPT::Parser spt_parser;
	Rut::RxJson::Value msg_json;

	spt_parser.Parse(wsScriptPath);
	Rut::RxJson::Parser{}.Load(wsJsonPath, msg_json);

	Rut::RxJson::JArray& msg_list = msg_json.ToAry();
	std::vector<GSD::SPT::SPT_Code>& code_list = spt_parser.GetCodeList();

	std::vector<std::vector<GSD::SPT::SPT_Code>::iterator> code_list_with_text;
	for (auto ite = code_list.begin(); ite != code_list.end(); ite++)
	{
		if (ite->GetParamType0().GetType0TextLen())
		{
			code_list_with_text.push_back(ite);
		}
	}

	if (code_list_with_text.size() != msg_list.size())
	{
		throw std::runtime_error("Mismatch text count");
	}

	for (size_t ite = 0; ite < msg_list.size(); ite++)
	{
		std::string text = Rut::RxStr::ToMBCS(msg_list[ite], nCodePage);
		code_list_with_text[ite]->GetParamType0().SetType0Text(text);
	}

	spt_parser.Dump().SaveData(wsScriptPath + L".new");
}

int main()
{
	try
	{
		ImportText(L"0scene_pro001.spt", L"0scene_pro001.spt.json", 936);
		//ExportText(L"0scene_pro001.spt", 932);
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}