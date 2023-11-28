#pragma once
#include <string>
#include <vector>

#include "GSD_Types.h"


namespace GSD
{
	class GSP
	{
	private:
		std::wstring m_wsPackPath;
		std::vector<GSP_Entry> m_vcIndex;

	public:
		GSP(const std::wstring_view wsPackPath);

		void ReadIndex();
		void Extract(const std::wstring& wsFolderPath);
	};
}