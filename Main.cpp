#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <string>
#include <psapi.h>

// Getting time in Double
double GetLastWriteTime(std::wstring FilePath) // Getting Last Write Time Obviously.
{

	HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile failed with " << GetLastError() << std::endl;
	}

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

	CloseHandle(hFile);

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
void CreateBackUpsFolder(std::wstring path)
{

	bool flag = CreateDirectory((GetDirOrName(path, 0) + L"\\BackUps").c_str(), NULL);
	if (!flag)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// it's okay
			std::cout << "You already have a BackUps folder" << std::endl;
		}
		else
		{
			// smth went wrong, we need a valid "path"
			std::cout << "Looks like your path is broken. We need a new one" << std::endl;
		}
	}
}

// Creating a backup
bool CreateBackUp(std::wstring path, double date)
{
	std::wstring name = GetDirOrName(path, 1);
	std::wstring dir = GetDirOrName(path, 0);
	std::wstring NewName = dir + L"\\BackUps\\" + name + std::to_wstring(date);

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
void UpdateFile(std::wstring from, std::wstring to, double date)
{
	if (CreateBackUp(to, date))
	{
		if (CopyFile(from.c_str(), to.c_str(), 0))
		{
			std::cout << "File has been successfully updated" << std::endl;
		}
		else
		{
			std::cout << "Updating a file has failed" << std::endl;
		}
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

	std::wstring File1Path = L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld"; // Our CloudFolder
	std::wstring File2Path = L"d:\\posos322.wld";

	double Time1;
	double Time2;

	int iStage = 0;
	for (;;)
	{
		if (!iStage) // Initialization
		{
			CreateBackUpsFolder(File1Path); // Making sure that we have it
			CreateBackUpsFolder(File2Path); // THIS IS A BULLSHIT, REMAKE THIS!!!
			if (GetPID()) // Checking if Terraria is running
						  // If yes, then we should wait until it's closed
			{
				iStage = 2;
			}
			else          // If no, then we can compare saves
			{
				Time1 = GetLastWriteTime(File1Path);
				Time2 = GetLastWriteTime(File2Path);
				if (Time1 > Time2)
				{
					UpdateFile(File1Path, File2Path, Time1);
				}
				else if (Time1 < Time2) // Should be impossible tbh, but just in case
				{
					UpdateFile(File2Path, File1Path, Time2);
				}
				iStage = 1;
			}
		}
		else if (iStage == 1) // Game isn't running
		{
//			Sleep(10000);
			if (GetPID())
			{
				iStage = 2;
				std::cout << "Going to Stage 2" << std::endl;
			}
		}
		else if (iStage == 2) // Game is running
		{
//			Sleep(10000);
			if (!GetPID())
			{
				iStage = 3;
				std::cout << "Going to Stage 3" << std::endl;
			}
		}
		else if (iStage == 3) // Game isn't running anymore
		{

			Time1 = GetLastWriteTime(File1Path);
			Time2 = GetLastWriteTime(File2Path);
			if (Time1 > Time2) // Should be impossible tbh, but just in case
			{
				UpdateFile(File1Path, File2Path, Time1);
			}
			else if (Time1 < Time2)
			{
				UpdateFile(File2Path, File1Path, Time2);
			}
			iStage = 1;
			std::cout << "Going back to Stage 1" << std::endl;
		}
	}

	system("PAUSE");
}