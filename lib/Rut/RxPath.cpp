#include "RxPath.h"
#include "RxStr.h"
#include "RxFile.h"
#include "Platform/Platform.h"

#include <queue>
#include <stdexcept>
#include <Windows.h>


namespace Rut::RxPath
{
	size_t FileName(char* cpPath, size_t nChar)
	{
		for (size_t iteChar = nChar - 1; iteChar > 0; iteChar--)
		{
			if (cpPath[iteChar] != '\\' && cpPath[iteChar] != '/')
			{
				continue;
			}

			if (cpPath[iteChar - 1] <= 0x7F)
			{
				size_t name_len = nChar - iteChar - 1;
				memcpy(cpPath, cpPath + iteChar + 1, name_len);
				cpPath[name_len] = '\0';
				return name_len;
			}
		}

		return nChar;
	}

	size_t FileName(wchar_t* wpPath, size_t nChar)
	{
		for (size_t iteChar = nChar - 1; iteChar > 0; iteChar--)
		{
			if (wpPath[iteChar] != L'\\' && wpPath[iteChar] != L'/')
			{
				continue;
			}

			size_t name_len = nChar - iteChar - 1;
			memcpy(wpPath, wpPath + iteChar + 1, name_len * 2);
			wpPath[name_len] = L'\0';
			return name_len;
		}

		return nChar;
	}

	std::string FileName(std::string msPath)
	{
		FileName(msPath.data(), msPath.size());
		return msPath;
	}

	std::wstring FileName(std::wstring wsPath)
	{
		FileName(wsPath.data(), wsPath.size());
		return wsPath;
	}


	size_t NotFileName(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '\\')
			{
				if (cpPath[ite_char - 1] <= 0x7F)
				{
					cpPath[ite_char + 1] = '\0';
					return ite_char + 1;
				}
			}

			if (cpPath[ite_char] == '/')
			{
				cpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}
		}

		return nChar;
	}

	size_t NotFileName(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'\\')
			{
				wpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}

			if (wpPath[ite_char] == L'/')
			{
				wpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}
		}

		return nChar;
	}

	std::string NotFileName(std::string msPath)
	{
		NotFileName(msPath.data(), msPath.size());
		return msPath;
	}

	std::wstring NotFileName(std::wstring msPath)
	{
		NotFileName(msPath.data(),msPath.size());
		return msPath;
	}


	size_t Suffix(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '.')
			{
				size_t suffix_len = nChar - ite_char + 1;
				memcpy(cpPath, cpPath + ite_char, suffix_len);
				return ite_char;
			}
		}

		return nChar;
	}

	size_t Suffix(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'.')
			{
				size_t suffix_len = nChar - ite_char + 1;
				memcpy(wpPath, wpPath + ite_char, suffix_len * sizeof(wchar_t));
				return ite_char;
			}
		}

		return nChar;
	}

	std::string Suffix(std::string msPath)
	{
		Suffix(msPath.data(), msPath.size());
		return msPath;
	}

	std::wstring Suffix(std::wstring wsPath)
	{
		Suffix(wsPath.data(), wsPath.size());
		return wsPath;
	}


	size_t NotSuffix(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '.')
			{
				cpPath[ite_char] = '\0';
				return ite_char;
			}
		}

		return nChar;
	}

	size_t NotSuffix(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'.')
			{
				wpPath[ite_char] = L'\0';
				return ite_char;
			}
		}

		return nChar;
	}

	std::string NotSuffix(std::string msPath)
	{
		NotSuffix(msPath.data(),msPath.size());
		return msPath;
	}

	std::wstring NotSuffix(std::wstring wsPath)
	{
		NotSuffix(wsPath.data(), wsPath.size());
		return wsPath;
	}


	char* FormatSlash(char* cpPath, char cSlash)
	{
		switch (cSlash)
		{
		case '\\':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if ((uint8_t)cpPath[ite] > 0x7F) { ite++; continue; }

				if (cpPath[ite] == '/') { cpPath[ite] = '\\'; }
			}
		}
		break;

		case '/':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if (cpPath[ite] == '\\') { cpPath[ite] = '/'; }
			}
		}
		break;
		}

		return cpPath;
	}

	wchar_t* FormatSlash(wchar_t* wpPath, wchar_t wcSlash)
	{
		switch (wcSlash)
		{
		case L'\\':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'/') { wpPath[ite] = L'\\'; }
			}
		}
		break;

		case L'/':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'\\') { wpPath[ite] = L'/'; }
			}
		}
		break;
		}

		return wpPath;
	}

	std::string FormatSlash(std::string msPath, char cSlash)
	{
		FormatSlash(msPath.data(), cSlash);
		return msPath;
	}

	std::wstring FormatSlash(std::wstring wsPath, wchar_t wcSlash)
	{
		FormatSlash(wsPath.data(), wcSlash);
		return wsPath;
	}
}

namespace Rut::RxPath
{

	bool Exist(std::string_view msPath)
	{
		return Platform::Exist(msPath.data());
	}

	bool Exist(std::wstring_view wsPath)
	{
		return Platform::Exist(wsPath.data());
	}


	bool DirExist(std::string_view msPath)
	{
		return Platform::DirExist(msPath.data());
	}

	bool DirExist(std::wstring_view wsPath)
	{
		return Platform::DirExist(wsPath.data());
	}


	bool FileExist(std::string_view msPath)
	{
		return Platform::FileExist(msPath.data());
	}

	bool FileExist(std::wstring_view wsPath)
	{
		return Platform::FileExist(wsPath.data());
	}


	bool MakeDir(std::string_view msPath)
	{
		return Platform::MakeDir(msPath.data());
	}

	bool MakeDir(std::wstring_view wsPath)
	{
		return Platform::MakeDir(wsPath.data());
	}


	static bool MakeDirViaPath(const char* cpPath)
	{
		return MakeDirViaPath(RxStr::ToWCS(cpPath, 0).c_str());
	}

	static bool MakeDirViaPath(const wchar_t* wpPath)
	{
		const size_t len = Platform::StrLen(wpPath);
		wchar_t path[Platform::PLATFORM_MAX_PATH];
		Platform::StrCpy(path, Platform::PLATFORM_MAX_PATH, wpPath);

		for (size_t ite_char = 0; ite_char < len; ite_char++)
		{
			switch (path[ite_char])
			{
			case L'/':
			case L'\\':
			{
				path[ite_char] = L'\0';
				Platform::MakeDir(path);
				path[ite_char] = L'\\';
			}
			break;

			case L'.':
			case L':': { ite_char++; } break;
			}
		}

		return true;
	}

	bool MakeDirViaPath(std::string_view msPath)
	{
		return MakeDirViaPath(msPath.data());
	}

	bool MakeDirViaPath(std::wstring_view wsPath)
	{
		return MakeDirViaPath(wsPath.data());
	}


	std::uintmax_t FileSize(std::string_view msPath)
	{
		return Platform::GetFileSize(msPath.data());
	}

	std::uintmax_t FileSize(std::wstring_view wsPath)
	{
		return Platform::GetFileSize(wsPath.data());
	}


	std::string ModulePathA(void* pBase)
	{
		std::string path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModulePath(path.data(), Platform::PLATFORM_MAX_PATH, pBase));
		return path;
	}

	std::wstring ModulePathW(void* pBase)
	{
		std::wstring path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModulePath(path.data(), Platform::PLATFORM_MAX_PATH, pBase));
		return path;
	}


	std::string ModuleNameA(void* pBase)
	{
		std::string name = ModulePathA(pBase);
		FileName(name.data(), name.size());
		return name;
	}

	std::wstring ModuleNameW(void* pBase)
	{
		std::wstring name = ModulePathW(pBase);
		FileName(name.data(), name.size());
		return name;
	}


	std::string ModuleDirA()
	{
		std::string path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModuleDir(path.data(), Platform::PLATFORM_MAX_PATH));
		return path;
	}

	std::wstring ModuleDirW()
	{
		std::wstring path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModuleDir(path.data(), Platform::PLATFORM_MAX_PATH));
		return path;
	}

}

namespace Rut::RxPath
{
	static void CheckPath(std::string& msPath)
	{
		FormatSlash(msPath.data(), '\\');
		if (msPath.back() != '\\') { msPath.append(1, '\\'); }
	}

	static void CheckPath(std::wstring& wsPath)
	{
		FormatSlash(wsPath.data(), L'\\');
		if (wsPath.back() != L'\\') { wsPath.append(1, L'\\'); }
	}

	bool AllFilePaths(std::string msPath, std::vector<std::string>& vecList)
	{
		CheckPath(msPath);

		std::queue<std::string> dirs;
		WIN32_FIND_DATAA find_data = { 0 };

		dirs.push(msPath.data());

		for (HANDLE hFind = INVALID_HANDLE_VALUE; !dirs.empty(); dirs.pop())
		{
			std::string& dirName = dirs.front();

			hFind = FindFirstFileA((dirName + "*").c_str(), &find_data);
			if (hFind == INVALID_HANDLE_VALUE) { return false; }

			do
			{
				if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push(dirName + find_data.cFileName + "\\");
					continue;
				}

				vecList.emplace_back(FormatSlash(dirName + find_data.cFileName, L'/'));

			} while (FindNextFileA(hFind, &find_data));

			FindClose(hFind);
		}

		return true;
	}

	bool AllFilePaths(std::wstring wsPath, std::vector<std::wstring>& vecList)
	{
		CheckPath(wsPath);

		std::queue<std::wstring> dirs;
		WIN32_FIND_DATAW find_data = { 0 };

		dirs.push(wsPath.data());

		for (HANDLE hfile = INVALID_HANDLE_VALUE; !dirs.empty(); dirs.pop())
		{
			std::wstring& dirName = dirs.front();

			hfile = FindFirstFileW((dirName + L"*").c_str(), &find_data);
			if (hfile == INVALID_HANDLE_VALUE) { return false; }

			do
			{
				if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push(dirName + find_data.cFileName + L"\\");
					continue;
				}

				vecList.emplace_back(FormatSlash(dirName + find_data.cFileName, L'/'));

			} while (FindNextFileW(hfile, &find_data));

			FindClose(hfile);
		}

		return true;
	}

	bool CurFileNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		CheckPath(msFolder);

		const std::string folder = msFolder.data();

		WIN32_FIND_DATAA find_data = { 0 };

		const HANDLE hfile = FindFirstFileA((folder + "*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			if (isAddBasePath)
			{
				vecList.emplace_back(FormatSlash(msFolder + find_data.cFileName, '/'));
			}
			else
			{
				vecList.emplace_back(FormatSlash(find_data.cFileName, '/'));
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFileNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		const std::wstring folder = wsFolder.data();

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((folder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			if (isAddBasePath)
			{
				vecList.emplace_back(FormatSlash(wsFolder + find_data.cFileName, L'/'));
			}
			else
			{
				vecList.emplace_back(FormatSlash(find_data.cFileName, L'/'));
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		CheckPath(msFolder);

		const std::string folder = msFolder.data();

		WIN32_FIND_DATAA find_data = { 0 };

		const HANDLE hfile = FindFirstFileA((folder + "*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(FormatSlash(msFolder + find_data.cFileName, '/'));
				}
				else
				{
					vecList.emplace_back(FormatSlash(find_data.cFileName, '/'));
				}
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((wsFolder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(FormatSlash(wsFolder + find_data.cFileName, L'/'));
				}
				else
				{
					vecList.emplace_back(FormatSlash(find_data.cFileName, L'/'));
				}
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}
}