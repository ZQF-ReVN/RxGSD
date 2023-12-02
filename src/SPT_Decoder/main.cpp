#include "../../lib/GSD/SPT.h"

#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxMem.h"

#include <vector>


void Decode()
{
	std::vector<std::wstring> file_list;
	Rut::RxPath::CurFileNames(L"data/", file_list, false);
	for (auto& file_name : file_list)
	{
		Rut::RxMem::Auto spt_file{ L"data/" + file_name };
		GSD::SPT::Coder::Decode(spt_file.GetPtr(), spt_file.GetSize(), true);
		spt_file.SaveData(L"data_dec/" + file_name);
	}
}


int main()
{
	Decode();
}