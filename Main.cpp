#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>


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

int main()
{
	HANDLE hFile1;
	HANDLE hFile2;
	TCHAR szBuf1[MAX_PATH];
	TCHAR szBuf2[MAX_PATH];

	hFile1 = CreateFile(L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	hFile2 = CreateFile(L"d:\\1.txt", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if ((hFile1 == INVALID_HANDLE_VALUE) || (hFile2 == INVALID_HANDLE_VALUE))
	{
		printf("CreateFile failed with %d\n", GetLastError());
		return 0;
	}

	if (GetLastWriteTime(hFile1, szBuf1, MAX_PATH) > GetLastWriteTime(hFile2, szBuf2, MAX_PATH))
	{
		std::cout << "First" << std::endl;
	}
	else
	{
		std::cout << "Second" << std::endl;
	}


	CloseHandle(hFile1);
	CloseHandle(hFile2);
	system("PAUSE");
}