#include <cstdio>
#include <list>
#include <iostream>
#include <sstream>
#include <cstring>

#define DIM 9

typedef int** sudokuBoard;

class SudokuConfig
{
public:
	int board[DIM][DIM];

	SudokuConfig(sudokuBoard _initial)
	{
		int** board_as_ptr = (int**)board;
		for (int r = 0; r < DIM; r++){
			for (int c = 0; c < DIM; c++){
				board[r][c] = _initial[r][c];
			}
		}
	}

	SudokuConfig(SudokuConfig* config)
	{
		for (int i = 0; i < DIM; i++){
			for (int j = 0; j < DIM; j++){
				board[i][j] = config->board[i][j];
			}
		}
	}

	friend std::ostream& operator<< (std::ostream &out, SudokuConfig &config);
	void values(int direction, int index, int array[]);
	bool isValid();
	bool isGoal();
	void getSuccessors(SudokuConfig* suc[], int* count);
	SudokuConfig* solve(bool debug);

};
int count(int* array, int value);
int getQuadrant(int row, int col);
int main(int argc, char** argv);






