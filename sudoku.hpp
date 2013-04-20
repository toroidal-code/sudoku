#include <cstdio>
#include <list>
#include <iostream>
#include <sstream>

#define DIM 9

class SudokuConfig
{
public:
	int** board;
	SudokuConfig(int** _initial)
	{
		board = _initial;
	}
	friend std::ostream& operator<< (std::ostream &out, SudokuConfig &config);
	void values(int direction, int index, int* *array);
	bool isValid();
	bool isGoal();
	void getSuccessors(SudokuConfig* array);
	SudokuConfig solve(bool debug);

};
int count(int* array, int value);
int getQuadrant(int row, int col);
int main(int argc, char** argv);






