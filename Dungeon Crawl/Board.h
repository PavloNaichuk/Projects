#pragma once

const int kSize = 9;

class Board
{
public:
	enum State
	{
		State_0 = 0,
		State_1,
		State_9, 
		State_Trap
	};

	Board();
	void Print() const;

	State GetCellState(int row, int col) const;
	void SetCellState(int row, int col, State state);
	void Reset();

private:
	State mCells[kSize][kSize];
};

