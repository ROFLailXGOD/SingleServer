#include "Save.h"
#include <string>
#include <windows.h>
#include <iostream>

bool Save::SetLWTime()
{
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile failed with " << GetLastError() << std::endl;
		return 0;
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
	time = dblt;

	return 1;
}

double Save::GetLWTime() const
{
	return time;
}

bool Save::SetPathAndName(std::wstring dir)
{
	path = dir;
	std::size_t i = dir.find_last_of(L"/\\");
	if (i >= 0)
	{
		name = dir.substr(i + 1); // Name
		return 1;
	}
	else
	{
		std::cout << "Your path is probably broken" << std::endl;
		return 0;
	}
}

std::wstring Save::GetPath() const
{
	return path;
}

std::wstring Save::GetName() const
{
	return name;
}

