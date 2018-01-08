//#include <windows.h>
#include <iostream>
#include <string>
#include "Save.h"
#include "Game.h"

int main()
{

	//std::wstring FilePath = L"C:\\Users\\vgork\\OneDrive\\Документы\\My Games\\Terraria\\Worlds\\posos322.wld";
	std::wstring FilePath = L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld";
	File File1;
	File1.SetPathAndName(FilePath);
	Application Terraria;
	Terraria.SetAppName(L"Terraria.exe");
	//Terraria.SetCloudFolder(L"C:\\Users\\Vgork\\OneDrive\\Terraria");
	Terraria.SetCloudFolder(L"C:\\Users\\Zabey\\OneDrive\\Terraria");

	int iStage = 0;
	for (;;)
	{
		if (!iStage) // Initialization
		{
			if (Terraria.GetPID()) // Checking if Application is running
								   // If yes, then we should wait until it's closed
			{
				iStage = 2;
			}
			else          // If no, then we can update saves
			{
				iStage = 1;
			}
		}
		else if (iStage == 1) // Application isn't running
		{
			Terraria.UpdateFile(File1);
			Sleep(10000);
			if (Terraria.GetPID())
			{
				iStage = 2;
				std::cout << "Going to Stage 2" << std::endl;
			}
		}
		else if (iStage == 2) // Application is running
		{
			Sleep(10000);
			if (!Terraria.GetPID())
			{
				iStage = 3;
				std::cout << "Going to Stage 3" << std::endl;
			}
		}
		else if (iStage == 3) // Application isn't running anymore
		{
			Terraria.UpdateFile(File1);
			iStage = 1;
			std::cout << "Going back to Stage 1" << std::endl;
		}
	}

	system("PAUSE");
}