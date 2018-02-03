#pragma once
#include <QMetaType>
#include <string>
#include <Windows.h>
class File;

class Application
{
private:
	std::wstring AppName; // Application name

	bool FindPID(DWORD) const; // Finding proper PID from array of PIDs
public:
	Application();
	Application(std::wstring);

	bool SetAppName(std::wstring);
    std::wstring GetAppName() const;

	bool IsRunning() const; // Looks through all PIDs
};
Q_DECLARE_METATYPE(Application)
