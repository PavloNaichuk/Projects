#include "Board.h"
#include <iostream>
#include <fstream>

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
				std::cout << '0';
			else if (mCells[row][col] == State_Player)
				std::cout << '1';
			else if (mCells[row][col] == State_Finish)
				std::cout << '9';
			else if (mCells[row][col] == State_Trap)
				std::cout << '*';
			else if (mCells[row][col] == State_Monster)
				std::cout << '!';
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
			mCells[row][col] = State_Free;
		}
	}
	mCells[0][0] = State_Player;
	mCells[kSize - 1][kSize - 1] = State_Finish;

	const int kNumTraps = 5;
	int trapIndex = kNumTraps;
	while (trapIndex > 0)
	{
		int row = rand() % kSize;
		int col = rand() % kSize;

		if (mCells[row][col] == State_Free)
		{
			mCells[row][col] = State_Trap;
			--trapIndex;
		}
	}

	const int kNumMonsters = 5;
	int monsterIndex = kNumMonsters;
	while (monsterIndex > 0)
	{
		int row = rand() % kSize;
		int col = rand() % kSize;

		if (mCells[row][col] == State_Free)
		{
			mCells[row][col] = State_Monster;
			--monsterIndex;
		}
	}
}

void Board::Save(const char* fileName) const
{	
	std::ofstream file(fileName);
	if (file)
	{
		for (int row = 0; row < kSize; ++row)
		{
			for (int col = 0; col < kSize; ++col)
				file << mCells[row][col];
		}
	}
}

void Board::Load(const char* fileName)
{
	std::ifstream file(fileName);
	if (file)
	{
		for (int row = 0; row < kSize; ++row)
		{
			for (int col = 0; col < kSize; ++col)
			{
				int state;
				file >> state;
				mCells[row][col] = (State)state;
			}
		}		
	}
}