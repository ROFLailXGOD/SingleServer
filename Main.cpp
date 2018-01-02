#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <string>
#include <psapi.h>

// Getting time in Double
double GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize) // Getting Last Write Time Obviously.
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;
	double dblt;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return 0;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToVariantTime(&stUTC, &dblt);
	return dblt;
}

// Getting directory of a file or a file name (flag = 0 to get directory, 1 to get name)
std::wstring GetDirOrName(std::wstring path, bool flag)
{
	int i = path.find_last_of(L"/\\");
	if (flag)
	{
		return path.substr(i + 1); // Name
	}
	else 
	{
		return path.substr(0, i); // Directory
	}
}

// Creating BackUpFolder only if not already exists
void CreateBackUpFolder(std::wstring path)
{

	bool flag = CreateDirectory((GetDirOrName(path, 0) + L"\\BackUp").c_str(), NULL);
	if (!flag)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// it's okay
			std::cout << "You already have a BackUp folder" << std::endl;
		}
		else
		{
			// smth went wrong, we need a valid "path"
			std::cout << "Looks like your path is broken. We need a new one" << std::endl;
		}
	}
}

// Creating a backup
void CreateBackUp(std::wstring path, double date)
{
	std::wstring name = GetDirOrName(path, 1);
	std::wstring dir = GetDirOrName(path, 0);
	std::wstring NewName = dir + L"\\BackUp\\" + name + std::to_wstring(date);

	if (CopyFile(path.c_str(), NewName.c_str(), 0))
	{
		std::cout << "New file has been successfully created" << std::endl;
	}
	else
	{
		std::cout << "Creating a new file has failed" << std::endl;
	}
}

// Finding proper PID from array of PIDs
bool FindPID(DWORD processID)
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

	// Searching for Terraria.exe
	if (_tcscmp(szProcessName, L"Terraria.exe") == 0)
	{
		std::cout << "Terraria.exe is running now!" << std::endl;
		CloseHandle(hProcess);
		return 1;
	}

	// Release the handle to the process.

	CloseHandle(hProcess);
	return 0;
}

// Getting Terraria.exe PID. Returns PID if Terraria is running. Otherwise returns 0
DWORD GetPID()
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


int main()
{
	HANDLE hFile1;
	HANDLE hFile2;
	TCHAR szBuf1[MAX_PATH];
	TCHAR szBuf2[MAX_PATH];
	std::wstring File1Path = L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld";
//	std::wstring File1Path = L"d:\\1.txt";

/*	hFile1 = CreateFile(File1Path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, 0, NULL);
	hFile2 = CreateFile(L"d:\\1.txt", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, 0, NULL);

	if ((hFile1 == INVALID_HANDLE_VALUE) || (hFile2 == INVALID_HANDLE_VALUE))
	{
		std::cout << "CreateFile failed with " << GetLastError() << std::endl;
	}


	double Time1 = GetLastWriteTime(hFile1, szBuf1, MAX_PATH);
	double Time2 = GetLastWriteTime(hFile2, szBuf2, MAX_PATH);
	if (Time1 > Time2)
	{
		std::cout << "First" << std::endl;
	}
	else
	{
		std::cout << "Second" << std::endl;
	}


	CloseHandle(hFile1);
	CloseHandle(hFile2);

	CreateBackUpFolder(File1Path);
	CreateBackUp(File1Path, Time1); */

	std::cout << GetPID() << std::endl;

	system("PAUSE");
}