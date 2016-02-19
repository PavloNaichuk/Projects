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
			if (mCells[row][col] == State_0)
				std::cout << '0';
			else if (mCells[row][col] == State_1)
				std::cout << '1';
			else if (mCells[row][col] == State_9)
				std::cout << '9';
			else if (mCells[row][col] == State_Trap)
				std::cout << '*';
			std::cout << ' ';
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

void Board::Reset()
{
	for (int row = 0; row < kSize; ++row)
	{
		for (int col = 0; col < kSize; ++col)
		{
			mCells[row][col] = State_0;
		}
	}
	mCells[0][0] = State_1;
	mCells[kSize - 1][kSize - 1] = State_9;

	const int kNumTraps = 5;
	int trapIndex = kNumTraps;
	while (trapIndex > 0)
	{
		int row = rand() % kSize;
		int col = rand() % kSize;

		if (mCells[row][col] == State_0)
		{
			mCells[row][col] = State_Trap;
			--trapIndex;
		}
	}
}