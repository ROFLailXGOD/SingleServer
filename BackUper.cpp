#include "BackUper.h"

BackUper::BackUper(){}

BackUper::BackUper(std::wstring path)
{
	cFile = File(path);
	bHasLinkedApp = false;
    bSynch = 0;
}

BackUper::BackUper(Application *objA, std::wstring path)
{
	cFile = File(path);
	App = objA;
	bHasLinkedApp = true;
    bSynch = 0;
}

File& BackUper::GetFile()
{
	return cFile;
}

Application BackUper::GetApp() const
{
    return *App;
}

void BackUper::SetSynch(bool arg)
{
    bSynch = arg;
}

bool BackUper::isSynched()
{
    return bSynch;
}
