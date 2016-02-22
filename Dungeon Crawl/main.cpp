#include <iostream>
#include <ctime>
#include <vector>
#include "Board.h"

std::vector<std::pair<int, int>> GetMonsterMoves(const Board& board, int row, int col)
{
	std::vector<std::pair<int, int>> moves;

	if ((row > 0) && (board.GetCellState(row - 1, col) == Board::State_Free))
		moves.push_back(std::make_pair(row - 1, col));
	
	if ((row < kSize - 1) && (board.GetCellState(row + 1, col) == Board::State_Free))
		moves.push_back(std::make_pair(row + 1, col));
	
	if ((col > 0) && (board.GetCellState(row, col - 1) == Board::State_Free))
		moves.push_back(std::make_pair(row, col - 1));
	
	if ((col < kSize - 1) && (board.GetCellState(row, col + 1) == Board::State_Free))
		moves.push_back(std::make_pair(row, col + 1));

	return moves;
}

void DungeonCrawl()
{
	srand(time(NULL));

	const int kUpArrow = 56;
	const int kDownArrow = 53;
	const int kLeftArrow = 52;
	const int kRightArrow = 54;

	int playerRow = 0;
	int playerCol = 0;

	Board board;
	board.Print();

	bool play = true;
	while (play)
	{
		std::cout << "Player move:\n";
		const int key = std::cin.get();
		std::cin.get();

		int playerNewRow = playerRow;
		int playerNewCol = playerCol;

		if (key == kLeftArrow)
		{
			if (playerCol > 0)
				playerNewCol = playerCol - 1;
		}
		else if (key == kUpArrow)
		{
			if (playerRow > 0)
				playerNewRow = playerRow - 1;
		}
		else if (key == kRightArrow)
		{
			if (playerCol < kSize - 1)
				playerNewCol = playerCol + 1;
		}
		else if (key == kDownArrow)
		{
			if (playerRow < kSize - 1)
				playerNewRow = playerRow + 1;
		}
		if ((playerNewRow != playerRow) || (playerNewCol != playerCol))
		{
			if (board.GetCellState(playerNewRow, playerNewCol) == Board::State_Trap ||
				board.GetCellState(playerNewRow, playerNewCol) == Board::State_Monster)
			{
				board.SetCellState(playerRow, playerCol, Board::State_Free);
				board.SetCellState(playerNewRow, playerNewCol, Board::State_Player);
				board.Print();

				std::cout << "You lost!" << "\n";
				break;
			}
			else if (board.GetCellState(playerNewRow, playerNewCol) == Board::State_Finish)
			{
				board.SetCellState(playerRow, playerCol, Board::State_Free);
				board.SetCellState(playerNewRow, playerNewCol, Board::State_Player);
				board.Print();

				std::cout << "You won!" << "\n";
				break;
			}
			else 
			{
				board.SetCellState(playerRow, playerCol, Board::State_Free);
				board.SetCellState(playerNewRow, playerNewCol, Board::State_Player);
				board.Print();

				playerRow = playerNewRow;
				playerCol = playerNewCol;
			}

			std::vector<std::pair<int, int>> monsterCells;
			for (int monsterRow = 0; monsterRow < kSize; ++monsterRow)
			{
				for (int monsterCol = 0; monsterCol < kSize; ++monsterCol)
				{
					if (board.GetCellState(monsterRow, monsterCol) == Board::State_Monster)
					{
						monsterCells.push_back(std::make_pair(monsterRow, monsterCol));
					}
				}
			}

			for (size_t index = 0; index < monsterCells.size(); ++index)
			{
				int monsterRow = monsterCells[index].first;
				int monsterCol = monsterCells[index].second;

				if (board.GetCellState(monsterRow, monsterCol) == Board::State_Monster)
				{
					std::vector<std::pair<int, int>> moves = GetMonsterMoves(board, monsterRow, monsterCol);
					size_t moveIndex = std::rand() % moves.size();
					const std::pair<int, int>& move = moves[moveIndex];

					board.SetCellState(monsterRow, monsterCol, Board::State_Free);
					board.SetCellState(move.first, move.second, Board::State_Monster);

					std::cout << (index + 1) << " monster move:\n";
					board.Print();

					if ((move.first == playerRow) && (move.second == playerCol))
					{
						std::cout << "You lost!" << "\n";
						play = false;
						break;
					}
				}
			}
		}
	}
	system("pause");
}


int main()
{
	DungeonCrawl();
	return 0;
}