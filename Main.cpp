//#include <windows.h>
#include <iostream>
#include <string>
#include "Save.h"
#include "Game.h"

int main()
{

	std::wstring FilePath = L"C:\\Users\\Zabey\\Documents\\My Games\\Terraria\\Worlds\\posos322.wld";
	Save File1;
	File1.SetPathAndName(FilePath);
	Game Terraria;
	Terraria.SetAppName(L"Terraria.exe");
	Terraria.SetCloudFolder(L"D:\\");

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
				Terraria.UpdateFile(File1);
				iStage = 1;
			}
		}
		else if (iStage == 1) // Game isn't running
		{
			Sleep(10000);
			if (Terraria.GetPID())
			{
				iStage = 2;
				std::cout << "Going to Stage 2" << std::endl;
			}
		}
		else if (iStage == 2) // Game is running
		{
			Sleep(10000);
			if (!Terraria.GetPID())
			{
				iStage = 3;
				std::cout << "Going to Stage 3" << std::endl;
			}
		}
		else if (iStage == 3) // Game isn't running anymore
		{
			Terraria.UpdateFile(File1);
			iStage = 1;
			std::cout << "Going back to Stage 1" << std::endl;
		}
	}

	system("PAUSE");
}