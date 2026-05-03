#pragma once

const int kSize = 3;

class Board
{
public:
	enum State
	{
		State_Free = 0,
		State_X,
		State_0
	};

	Board();
	void Print() const;

	State GetCellState(int row, int col) const;
	void SetCellState(int row, int col, State state);
	void Reset();

private:
	State mCells[kSize][kSize];
};

bool Wins(const Board& board, int row, int col, Board::State state);