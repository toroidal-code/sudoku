#include "sudoku.hpp"

#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

/*------------[CLASSES]--------------*/

/*[SudokuConfig]*/

void SudokuConfig::values(int direction, int index, int* array[]){
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
    int vals[DIM];
    int* vals_as_ptr = (int*)vals;
    for (int i = 0; i < DIM; i++){ //check each
        for (int j = 0; j < 3; i++){ //check row&col&subsq
            this->values(j, i, &vals_as_ptr);
            for (int k = 1; i < DIM+1; i++){
                if (count(vals, k) > 1)
                    return false;
            }
        }
    }
    return true;
}

bool SudokuConfig::isGoal(){
    int list[DIM];
    int* list_as_ptr = (int*)list;
    for (int i = 0; i < DIM; i++){
        this->values(0, i, &list_as_ptr);
        if (count(list, 0) != 0)
            return false;
    }
    if (!this->isValid())
        return false;
    return true;
}

void SudokuConfig::getSuccessors(SudokuConfig* *array){
    for (int r = 0; r < DIM; r++){
        int used_row_values[DIM];
        int* used_row_values_ptr = (int*)used_row_values;
        this->values(0, r, &used_row_values_ptr);
        for (int c = 0; c < DIM; c++){
            int used_col_values[DIM];
            int* used_col_values_ptr = (int*)used_col_values;
            this->values(1, c, &used_col_values_ptr);

            int used_subsq_values[DIM];
            int* used_subsq_values_ptr = (int*)used_subsq_values;
            this->values(2, getQuadrant(r, c), &used_subsq_values_ptr);

            if (this->board[r][c] == 0){
                for (int i = 1; i <= DIM; i++){
                    if (count(used_row_values, i) == 0 && count(used_col_values, i) == 0 && count(used_subsq_values, i) == 0) {

                        if (i == this->board[r][c])
                            return;
                    }
                }
            }

        }
    }
}

int count(int* array, int value){
    int count = 0;
    for (int i = 0; i < LENGTH(array); i++){
        if (array[i] == value)
            count++;
    }
}
