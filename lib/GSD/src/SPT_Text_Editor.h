#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "GSD_Types.h"
#include "../../Rut/RxMem.h"
#include "../../Rut/RxFile.h"


namespace GSD::SPT
{
	class Text_Editor
	{
	private:
		std::wstring m_wsPath;
		Rut::RxMem::Auto m_amSPT;
		std::vector<SPT_Text_Entry*> m_vcTextEntryPtr;

	private:
		static std::wstring NumToHexStr(const size_t nValue);
		static size_t HexStrToNum(const std::wstring_view& wsText);
		static size_t DBCSCountChar(std::string_view msText);
		static std::wstring ReadText(SPT_Text_Entry* pEntry);
		static void MakeStruct(Rut::RxMem::Auto& rfMem, const std::string& msText);
		static void FileSwapData(Rut::RxFile::Binary& fsSource, size_t nBegPos, size_t nEndPos, Rut::RxFile::Binary& fsDest, Rut::RxMem::Auto& rfBuffer);

	public:
		Text_Editor(std::wstring_view wsPath);

		void ReadEntry();
		void Extract(const std::wstring_view wsJson);
		void Insert(const std::wstring_view wsJson);
	};
}