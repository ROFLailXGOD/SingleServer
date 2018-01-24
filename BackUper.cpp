#include "BackUper.h"

BackUper::BackUper(std::wstring path)
{
	cFile = File(path);
	bHasLinkedApp = false;
}

BackUper::BackUper(const Application &objA, std::wstring path)
{
	cFile = File(path);
	App = objA;
	bHasLinkedApp = true;
}

File& BackUper::GetFile()
{
	return cFile;
}

Application BackUper::GetApp() const
{
	return App;
}