#include "Board.h"
#include <iostream>

Board::Board()
{
	Reset();
}

void Board::Print() const
{
	for (int row = 0; row < kSize; ++row)
	{
		for (int col = 0; col < kSize; ++col)
		{
			if (mCells[row][col] == State_Free)
				std::cout << '-';
			else if (mCells[row][col] == State_X)
				std::cout << 'X';
			else if (mCells[row][col] == State_0)
				std::cout << '0';
		}
		std::cout << "\n";
	}
}

Board::State Board::GetCellState(int row, int col) const
{
	return mCells[row][col];
}

void Board::SetCellState(int row, int col, State state)
{
	mCells[row][col] = state;
}

bool Wins(const Board& board, int row, int col, Board::State state)
{
	int c = 0;
	for (; c < kSize; ++c)
	{
		if (board.GetCellState(row, c) != state)
			break;
	}
	if (c == kSize)
		return true;

	int r = 0;
	for (; r < kSize; ++r)
	{
		if (board.GetCellState(r, col) != state)
			break;
	}
	if (r == kSize)
		return true;
	
	if (row == col)
	{
		int d = 0;
		for (; d < kSize; ++d)
		{
			if (board.GetCellState(d, d) != state)
				break;
		}
		if (d == kSize)
			return true;
	}

	if (row + col == kSize - 1)
	{
		int r = 0;
		for (; r < kSize; ++r)
		{
			if (board.GetCellState(r, kSize - 1 - r) != state)
				break;
		}
		if (r == kSize)
			return true;
	}

	return false;
}
void Board::Reset()
{
	for (int row = 0; row < kSize; ++row)
	{
		for (int col = 0; col < kSize; ++col)
		{
			mCells[row][col] = State_Free;
		}
	}
}