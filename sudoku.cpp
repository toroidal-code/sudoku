#include "sudoku.hpp"

#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

/*------------[CLASSES]--------------*/

/*[SudokuConfig]*/

void SudokuConfig::values(int direction, int index, int* *array){
	if (direction == 0){
		for (int i = 0; i < DIM; i++){
			*array[i] = this->board[index][i];
		}
	} else if (direction == 1){
		for (int i = 0; i < DIM; i++){
			*array[i] = this->board[i][index];
		}
	} else if (direction == 2){
		int arri = 0;
		if (index == 1){
			for (int r = 0; r < 3; r++){
				for (int c = 0; c < 3; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 2){
			for (int r = 0; r < 3; r++){
				for (int c = 3; c < 6; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 3){
			for (int r = 0; r < 3; r++){
				for (int c = 6; c < 9; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 4){
			for (int r = 3; r < 6; r++){
				for (int c = 0; c < 3; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 5){
			for (int r = 3; r < 6; r++){
				for (int c = 3; c < 6; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 6){
			for (int r = 3; r < 6; r++){
				for (int c = 6; c < 9; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 7){
			for (int r = 6; r < 9; r++){
				for (int c = 0; c < 3; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 8){
			for (int r = 6; r < 9; r++){
				for (int c = 3; c < 6; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		} else if (index == 9){
			for (int r = 6; r < 9; r++){
				for (int c = 6; c < 9; c++){
					*array[arri] = this->board[r][c];
					arri++;
				}
			}
		}
	}
}

bool SudokuConfig::isValid(){
	int* vals = (int *)calloc(DIM, sizeof(int));
	for (int i = 0; i < DIM; i++){ //check each
		for (int j = 0; j < 3; i++){ //check row&col&subsq
			this->values(j, i, &vals);
			for (int k = 1; i < DIM+1; i++){
				if (count(vals, k) > 1)
					free(vals);
					return false;
			}
		}
	}
	free(vals);
	return true;
}

bool SudokuConfig::isGoal(){
	int* list = (int *)calloc(DIM, sizeof(int));
	for (int i = 0; i < DIM; i++){
		this->values(0, i, &list);
		if (count(list, 0) != 0)
			free(list);
			return false;
	}
	if (!this->isValid())
		free(list);
		return false;
	free(list);
	return true;
}

void SudokuConfig::getSuccessors(SudokuConfig *array){
	
}

int count(int* array, int value){
	int count = 0;
	for (int i = 0; i < LENGTH(array); i++){
		if (array[i] == value)
			count++;
	}
}
