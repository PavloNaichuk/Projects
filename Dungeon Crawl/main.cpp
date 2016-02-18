#include <iostream>
#include <string>
#include <cstdlib>
#include "Board.h"
#include <ctime>
#include <vector>

void DungeonCrawl()
{
	const int kUpArrow = 56;
	const int kDownArrow = 53;
	const int kLeftArrow = 52;
	const int kRightArrow = 54;

	int row = 0;
	int col = 0;

	Board board;
	board.Print();

	while (true)
	{
		const int key = std::cin.get();

		int newRow = row;
		int newCol = col;

		if (key == kLeftArrow)
		{
			if (col > 0)
				newCol = col - 1;
		}
		else if (key == kUpArrow)
		{
			if (row > 0)
				newRow = row - 1;
		}
		else if (key == kRightArrow)
		{
			if (col < kSize - 1)
				newCol = col + 1;
		}
		else if (key == kDownArrow)
		{
			if (row < kSize - 1)
				newRow = row + 1;
		}
		if ((newRow != row) || (newCol != col))
		{
			if (board.GetCellState(newRow, newCol) == Board::State_9)
			{
				board.SetCellState(row, col, Board::State_0);
				board.SetCellState(newRow, newCol, Board::State_1);
				board.Print();
				
				std::cout << "You win!" << "\n";
				break;
			}
			else
			{
				board.SetCellState(row, col, Board::State_0);
				board.SetCellState(newRow, newCol, Board::State_1);
				board.Print();
			}
			row = newRow;
			col = newCol;
		}
	}
	system("pause");
}


int main()
{
	DungeonCrawl();
	return 0;
}