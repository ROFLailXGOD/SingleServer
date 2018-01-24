#pragma once
#include "File.h"
#include "Application.h"

class BackUper
{
private:
	File cFile; // Linked file
	Application App; // Linked application. May exist, may not
public:
	bool bHasLinkedApp; // Is file linked with an application?

	BackUper(std::wstring);
	BackUper(const Application&, std::wstring);

	File GetFile() const;
	Application GetApp() const;
};
