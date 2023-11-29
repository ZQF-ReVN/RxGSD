#include "../../lib/GSD/SPT.h"


int main()
{
	GSD::SPT::Text_Editor editor(L"0scene_pro001.spt");
	editor.Extract(L"0scene_pro001.spt.json");
	// editor.Insert(L"0scene_pro001.spt.json");
}