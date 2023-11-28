#pragma once
#include <string>
#include <vector>


namespace Rut::RxPath
{
	size_t FileName(char* cpPath, size_t nChar);
	size_t FileName(wchar_t* wpPath, size_t nChar);
	std::string FileName(std::string msPath);
	std::wstring FileName(std::wstring wsPath);

	size_t NotFileName(char* cpPath, size_t nChar);
	size_t NotFileName(wchar_t* wpPath, size_t nChar);
	std::string NotFileName(std::string msPath);
	std::wstring NotFileName(std::wstring wsPath);

	size_t Suffix(char* cpPath, size_t nChar);
	size_t Suffix(wchar_t* wpPath, size_t nChar);
	std::string Suffix(std::string msPath);
	std::wstring Suffix(std::wstring wsPath);

	size_t NotSuffix(char* cpPath, size_t nChar);
	size_t NotSuffix(wchar_t* wpPath, size_t nChar);
	std::string NotSuffix(std::string msPath);
	std::wstring NotSuffix(std::wstring wsPath);

	char* FormatSlash(char* cpPath, char cSlash);
	wchar_t* FormatSlash(wchar_t* wpPath, wchar_t wcSlash);
	std::string FormatSlash(std::string msPath, char cSlash);
	std::wstring FormatSlash(std::wstring wsPath, wchar_t wcSlash);

	bool Exist(std::string_view msPath);
	bool Exist(std::wstring_view wsPath);

	bool DirExist(std::string_view msPath);
	bool DirExist(std::wstring_view wsPath);

	bool FileExist(std::string_view msPath);
	bool FileExist(std::wstring_view wsPath);

	bool MakeDir(std::string_view msPath);
	bool MakeDir(std::wstring_view wsPath);

	bool MakeDirViaPath(std::string_view msPath);
	bool MakeDirViaPath(std::wstring_view wsPath);

	std::uintmax_t FileSize(std::string_view msPath);
	std::uintmax_t FileSize(std::wstring_view wsPath);

	std::string  ModuleDirA();
	std::wstring ModuleDirW();

	std::string  ModulePathA(void* pBase = nullptr);
	std::wstring ModulePathW(void* pBase = nullptr);

	std::string ModuleNameA(void* pBase = nullptr);
	std::wstring ModuleNameW(void* pBase = nullptr);
}

namespace Rut::RxPath
{
	bool AllFilePaths(std::string msBasePath, std::vector<std::string>& vecList);
	bool AllFilePaths(std::wstring wsBasePath, std::vector<std::wstring>& vecList);

	bool CurFileNames(std::string msBasePath, std::vector<std::string>& vecList, bool isAddBasePath = false);
	bool CurFileNames(std::wstring wsBasePath, std::vector<std::wstring>& vecList, bool isAddBasePath = false);

	bool CurFolderNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath = false);
	bool CurFolderNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath = false);
}
