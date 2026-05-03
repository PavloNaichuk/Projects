#include "GameMode.h"
#include <iostream>

int main()
{
	while (true)
	{
		std::cout << "Please select game mode:\n";
		std::cout << "1 - Player vs Player\n";
		std::cout << "2 - Player vs PC\n";

		char key;
		std::cin >> key;

		if (key == '1')
		{
			PlayVsAnotherPlayer();
			break;
		}			
		if (key == '2')
		{
			PlayVsPC();
			break;
		}
		
		std::cout << "Invalid game mode!\n";
	}
	return 0;
}