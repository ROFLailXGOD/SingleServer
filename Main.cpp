//#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "Application.h"
#include "File.h"
#include "BackUper.h"

int main()
{
	std::vector<BackUper> arr;
	arr.reserve(100);
	Application App(L"Terraria.exe");
	BackUper File(App, L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld");
	(File.GetFile()).SetCloudFolder(L"C:\\Users\\Zabey\\OneDrive\\Terraria");
	arr.push_back(File);

	for (;;)
	{
		for (int i = 0; i < arr.size(); ++i)
		{
			if (arr[i].bHasLinkedApp)
			{
				if (!arr[i].GetApp().IsRunning()) // Making sure that App isn't running
				{
					(arr[i].GetFile()).UpdateFile();
				}
			}
			else
			{
				(arr[i].GetFile()).UpdateFile();
			}
		}
		Sleep(5000);
	}

	system("PAUSE");
}