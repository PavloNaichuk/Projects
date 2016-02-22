#pragma once

const int kSize = 9;

class Board
{
public:
	enum State
	{
		State_Free = 0,
		State_Player,
		State_Finish, 
		State_Trap,
		State_Monster
	};

	Board();
	void Print() const;

	State GetCellState(int row, int col) const;
	void SetCellState(int row, int col, State state);
	void Reset();

private:
	State mCells[kSize][kSize];
};

