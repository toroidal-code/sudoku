#include <cstdio>
#include <list>
#include <iostream>
#include <sstream>

#define DIM 9

class SudokuConfig
{
public:
	int board[DIM][DIM];
	SudokuConfig(int** _initial)
	{
		int** board_as_ptr = (int**)board;
		board_as_ptr = _initial;
	}
	SudokuConfig(SudokuConfig &config)
	{
		for (int i = 0; i < DIM; i++){
			for (int j = 0; j < DIM; j++){
				board[i][j] = config.board[i][j];
			}
		}
	}
	friend std::ostream& operator<< (std::ostream &out, SudokuConfig &config);
	void values(int direction, int index, int* *array);
	bool isValid();
	bool isGoal();
	void getSuccessors(SudokuConfig* *array);
	SudokuConfig solve(bool debug);

};
int count(int* array, int value);
int getQuadrant(int row, int col);
int main(int argc, char** argv);






