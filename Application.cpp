#include "Application.h"
#include "File.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <psapi.h>
#include <tchar.h>

Application::Application(){}

Application::Application(std::wstring name)
{
	SetAppName(name);
}

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
bool Application::IsRunning() const
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	DWORD cPID = false;
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
				cPID = true;
				break; // Found one, no need to search further
			}
		}
	}

	return cPID;
}