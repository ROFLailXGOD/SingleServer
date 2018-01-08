#include "Game.h"
#include "Save.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <psapi.h>
#include <tchar.h>

bool Application::SetAppName(std::wstring name)
{
	std::size_t i = name.find(L".exe");
	if (i >= 0)
	{
		AppName = name;
		return 1;
	}
	else
	{
		std::cout << "Your Application name is incorrect" << std::endl;
		return 0;
	}
}

bool Application::SetCloudFolder(std::wstring path)
{
	std::size_t i = path.find_last_of(L"/\\");
	if (i >= 0)
	{
		CloudFolder = path;
		BackUpsFolder = path + L"\\BackUps";
		CreateBackUpsFolder();
		return 1;
	}
	else
	{
		std::cout << "Your path is broken" << std::endl;
		return 0;
	}
}

bool Application::SetBackUpsFolder(std::wstring path)
{
	std::size_t i = path.find_last_of(L"/\\");
	if (i >= 0)
	{
		BackUpsFolder = path;
		return 1;
	}
	else
	{
		std::cout << "Your path is broken" << std::endl;
		return 0;
	}
}

// Creating BackUpFolder only if not already exists
bool Application::CreateBackUpsFolder() const
{
	bool flag = CreateDirectory(BackUpsFolder.c_str(), NULL);
	if (!flag)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// it's okay
			std::cout << "You already have a BackUps folder" << std::endl;
			return 1;
		}
		else
		{
			// smth went wrong, we need a valid "path"
			std::cout << "Looks like your path is broken. We need a new one" << std::endl;
			return 0;
		}
	}
	std::cout << "BackUps folder was successfully created" << std::endl;
	return 1;
}

// Creating a backup
bool Application::CreateBackUp(const File &SaveFile) const
{
	std::wstring name = SaveFile.GetName();
	std::wstring path = SaveFile.GetPath();
	std::wstring NewName = BackUpsFolder + L"\\" + name + std::to_wstring(SaveFile.GetLWTime());

	if (CopyFile(path.c_str(), NewName.c_str(), 0))
	{
		std::cout << "New backup has been successfully created" << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Creating a new backup has failed" << std::endl;
		return 0;
	} 
}

// Updating files
bool Application::UpdateFile(File &SaveFile) const
{
	std::wstring name = SaveFile.GetName();
	std::wstring NewName = CloudFolder + L"\\" + name;
	File CloudSave;
	CloudSave.SetPathAndName(NewName);
	CloudSave.SetLWTime();
	SaveFile.SetLWTime();
	if (CreateBackUp(SaveFile))
	{
		if (SaveFile.GetLWTime() > CloudSave.GetLWTime())
		{

			if (CopyFile(SaveFile.GetPath().c_str(), CloudSave.GetPath().c_str(), 0))
			{
				std::cout << "File has been successfully updated" << std::endl;
				return 1;
			}
			else
			{
				std::cout << "Updating a file has failed" << std::endl;
				return 0;
			}
		}
		else if (SaveFile.GetLWTime() < CloudSave.GetLWTime())
		{
			if (CopyFile(CloudSave.GetPath().c_str(), SaveFile.GetPath().c_str(), 0))
			{
				std::cout << "File has been successfully updated" << std::endl;
				return 1;
			}
			else
			{
				std::cout << "Updating a file has failed" << std::endl;
				return 0;
			}
		}
		else
		{
			std::wcout << "No need to update " << SaveFile.GetName() << std::endl;
		}
	}
}

// Finding proper PID from array of PIDs
bool Application::FindPID(DWORD processID) const
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	// Searching for AppName
	if (_tcscmp(szProcessName, AppName.c_str()) == 0)
	{
		std::wcout << AppName << " is running now!" << std::endl;
		CloseHandle(hProcess);
		return 1;
	}

	// Release the handle to the process.

	CloseHandle(hProcess);
	return 0;
}

// Getting Application PID. Returns PID if Application is running. Otherwise returns 0
DWORD Application::GetPID() const
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	DWORD cPID = 0;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}


	// Calculate how many process identifiers were returned

	cProcesses = cbNeeded / sizeof(DWORD);

	// Cycle through every single process to find the right one (if exists)

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			if (FindPID(aProcesses[i]))
			{
				cPID = aProcesses[i];
				break; // Found one, no need to search further
			}
		}
	}

	return cPID;
}