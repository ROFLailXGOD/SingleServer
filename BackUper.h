#pragma once
#include<QMetaType>
#include "File.h"
#include "Application.h"

class BackUper
{
private:
    File cFile; // Linked file
    Application *App; // Linked application. May exist, may not
    bool bSynch;
public:
	bool bHasLinkedApp; // Is file linked with an application?

    BackUper();
	BackUper(std::wstring);
    BackUper(Application*, std::wstring);

	File& GetFile();
	Application GetApp() const;

    bool isSynched(); // Is file synched?
};
Q_DECLARE_METATYPE(BackUper*)
