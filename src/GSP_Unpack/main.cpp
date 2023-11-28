#include "../../lib/GSD/GSP.h"


int main()
{
	GSD::GSP gsp{ L"data.gsp" };
	gsp.ReadIndex();
	gsp.Extract(L"data/");
}