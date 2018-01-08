#pragma once
#include <string>
#include <Windows.h>
class File;

class Application
{
private:
	std::wstring AppName; // Application name
	std::wstring CloudFolder; // Cloud Folder Path
	std::wstring BackUpsFolder; // BackUps folder Path
	bool CreateBackUpsFolder() const; // Creating BackUpFolder only if not already exists
	bool CreateBackUp(const File&) const; // Creating a backup
	bool FindPID(DWORD) const; // Finding proper PID from array of PIDs
public:
	bool UpdateFile(File&) const; // Updating files
	DWORD GetPID() const;
	bool SetAppName(std::wstring);
	bool SetCloudFolder(std::wstring);
	bool SetBackUpsFolder(std::wstring);
};