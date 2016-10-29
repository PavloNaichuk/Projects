#include "GameMode.h"
#include "Board.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

void PlayVsAnotherPlayer()
{
	std::string playerNames[2];
	const Board::State moveStates[] = { Board::State_X, Board::State_0 };

	int playerIndex = 0;
	int numMoves = 0;
	const int maxMoves = kSize * kSize;

	std::cout << "1 Player login: ";
	std::cin >> playerNames[0];

	std::cout << "2 Player login: ";
	std::cin >> playerNames[1];
	int row;
	int col;

	bool startNewGame = true;
	do
	{
		Board board;
		board.Print();
		while (true)
		{
			std::cout << playerNames[playerIndex] << " move:\n";

			while (true)
			{
				std::cout << "Row: ";
				std::cin >> row;

				std::cout << "Col: ";
				std::cin >> col;

				if ((board.GetCellState(row, col) == Board::State_Free) &&
					(row < kSize) && (row >= 0) &&
					(col < kSize) && (col >= 0))
				{
					board.SetCellState(row, col, moveStates[playerIndex]);
					break;
				}
				else
					std::cout << "Error. Please retry again" << "\n";
			}
			board.Print();

			if (Wins(board, row, col, moveStates[playerIndex]))
			{
				std::cout << playerNames[playerIndex] << " wins" << "\n";
				break;
			}

			++numMoves;
			if (numMoves == maxMoves)
			{
				std::cout << "Oh, just draw :-(\n";
				break;
			}
			playerIndex = (playerIndex + 1) % 2;
		}

		std::cout << "Do you want restart?(r/x)" << "\n";
		while (true)
		{
			char choice;
			std::cin >> choice;
			if (choice == 'r')
			{
				board.Reset();
				numMoves = 0;
				break;
			}
			if (choice == 'x')
			{
				startNewGame = false;
				break;
			}
			std::cout << "You enter wrong character!" << "\n";
		}
	} while (startNewGame);
	system("pause");
}

void PlayVsPC()
{
	srand(time(NULL));

	std::string playerNames[2];
	const Board::State moveStates[] = { Board::State_X, Board::State_0 };

	int playerIndex = 0;
	int numMoves = 0;
	const int maxMoves = kSize * kSize;

	std::cout << "Player login: ";
	std::cin >> playerNames[0];

	playerNames[1] = "PC";

	bool startNewGame = true;
	while (startNewGame)
	{
		Board board;
		board.Print();

		while (true)
		{
			std::cout << playerNames[playerIndex] << " move:\n";

			int row;
			int col;
			if (playerIndex == 0)
			{
				while (true)
				{
					std::cout << "Row: ";
					std::cin >> row;

					std::cout << "Col: ";
					std::cin >> col;

					if ((board.GetCellState(row, col) == Board::State_Free) &&
						(row < kSize) && (row >= 0) &&
						(col < kSize) && (col >= 0))
						break;
					else
						std::cout << "Error. Please retry again\n";
				}
			}
			else
			{
				std::vector<std::pair<int, int>> freeCells;
				for (int row = 0; row < kSize; ++row)
				{
					for (int col = 0; col < kSize; ++col)
					{
						if ((board.GetCellState(row, col) == Board::State_Free))
							freeCells.push_back(std::make_pair(row, col));
					}
				}

				int index = rand() % freeCells.size();
				const std::pair<int, int>& cell = freeCells[index];

				row = cell.first;
				col = cell.second;
			}
			board.SetCellState(row, col, moveStates[playerIndex]);
			board.Print();

			if (Wins(board, row, col, moveStates[playerIndex]))
			{
				std::cout << playerNames[playerIndex] << " wins" << "\n";
				break;
			}

			++numMoves;
			if (numMoves == maxMoves)
			{
				std::cout << "Oh, just draw :-(\n";
				break;
			}
			playerIndex = (playerIndex + 1) % 2;
		}

		std::cout << "Do you want restart?(r/x)" << "\n";
		while (true)
		{
			char choice;
			std::cin >> choice;
			if (choice == 'r')
			{
				board.Reset();
				numMoves = 0;
				break;
			}
			if (choice == 'x')
			{
				startNewGame = false;
				break;
			}
			std::cout << "You enter wrong character!" << "\n";
		}
	}
	system("pause");
}
