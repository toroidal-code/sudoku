#include "sudoku.hpp"

#define LENGTH(x) (sizeof(x)/sizeof(*(x))) 

typedef int** sudokuBoard;

/*------------[CLASSES]--------------*/

/*[SudokuConfig]*/

void SudokuConfig::values(int direction, int index, int array[]){
    if (direction == 0){
        for (int i = 0; i < DIM; i++){
            *(array + i) = this->board[index][i];
        }
    } else if (direction == 1){
        for (int i = 0; i < DIM; i++){
            *(array + i) = this->board[i][index];
        }
    } else if (direction == 2){
        int arri = 0;
        if (index == 1){
            for (int r = 0; r < 3; r++){
                for (int c = 0; c < 3; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 2){
            for (int r = 0; r < 3; r++){
                for (int c = 3; c < 6; c++){
                    *(array + arri) = this->board[r][c];
                    arri++; 
                }
            }
        } else if (index == 3){
            for (int r = 0; r < 3; r++){
                for (int c = 6; c < 9; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 4){
            for (int r = 3; r < 6; r++){
                for (int c = 0; c < 3; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 5){
            for (int r = 3; r < 6; r++){
                for (int c = 3; c < 6; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 6){
            for (int r = 3; r < 6; r++){
                for (int c = 6; c < 9; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 7){
            for (int r = 6; r < 9; r++){
                for (int c = 0; c < 3; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 8){
            for (int r = 6; r < 9; r++){
                for (int c = 3; c < 6; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        } else if (index == 9){
            for (int r = 6; r < 9; r++){
                for (int c = 6; c < 9; c++){
                    *(array + arri) = this->board[r][c];
                    arri++;
                }
            }
        }
    }
}

bool SudokuConfig::isValid(){
    int vals[DIM];
    for (int i = 0; i < DIM; i++){ //check each
        for (int j = 0; j < 3; j++){ //check row&col&subsq
            this->values(j, i, vals);
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
    for (int i = 0; i < DIM; i++){
        this->values(0, i, list);
        if (count(list, 0) != 0)
            return false;
    }
    if (!this->isValid())
        return false;
    return true;
}

void SudokuConfig::getSuccessors(SudokuConfig* suc[], int *num){
    for (int r = 0; r < DIM; r++){
        int used_row_values[DIM];
        this->values(0, r, used_row_values);
        for (int c = 0; c < DIM; c++){
            int used_col_values[DIM];
            this->values(1, c, used_col_values);

            int used_subsq_values[DIM];
            this->values(2, getQuadrant(r, c), used_subsq_values);

            if (this->board[r][c] == 0){
                for (int i = 1; i <= DIM; i++){
                    if (count(used_row_values, i) == 0 && count(used_col_values, i) == 0 && count(used_subsq_values, i) == 0) {

                        if (i == this->board[r][c])
                            return;
                        SudokuConfig* newConfig = new SudokuConfig(this);
                        newConfig->board[r][c] = i;
                        *(suc + (*num)++) = newConfig;
                    }
                }
                return;
            }

        }
    }
}

SudokuConfig* SudokuConfig::solve(bool debug){
    if (this->isGoal())
        return this;
    else{
        if (debug)
            std::cout << *this << std::endl;
        int count = 0;
        SudokuConfig* *successors = (SudokuConfig**)calloc(DIM, sizeof(SudokuConfig*));
        this->getSuccessors(successors, &count);
        for (int i = 0; i < count; i++){
            // if (debug)
            //     std::cout << *(successors + i) << std::endl;
            SudokuConfig* back = (*(successors + i))->solve(debug);
            if (back != NULL) {
                SudokuConfig* solution = new SudokuConfig(back);
                return solution;
            }
        }
    }
    return NULL;
}

std::ostream& operator<<(std::ostream &os, SudokuConfig &config){
    os << "\n " << std::string(DIM*2+5, '-') << '\n';

    for (int r = 0; r < DIM; r++){
        if (r == 3 || r == 6)
            os <<  '|' << std::string(DIM*2+5, '-') << '|' << '\n';
        os << "| ";
        for (int c = 0; c < DIM; c++){
            if (c == 3 || c == 6)
                os << "| ";
            if (config.board[r][c] == 0)
                os << '.';
            else
                os << config.board[r][c];
            if (c != DIM-1)
                os << ' ';
        }
        os << " |" << '\n';
    }
    os << ' ' << std::string(DIM*2+5, '-') << '\n';
    os << "  ";
    os << '\n';
    return os;
}

int getQuadrant(int row, int col){
    if (row < 3){
        if (col < 3)
            return 1;
        if (3 <= col && col < 6)
            return 2;
        if (col >= 6)
            return 3;
    } else if (3 <= row && row < 6){
        if (col < 3)
            return 4;
        if (3 <= col && col < 6)
            return 5;
        if (col >= 6)
            return 6;
    } else if (row >= 6){
        if (col < 3)
            return 7;
        if (3 <= col && col < 6)
            return 8;
        if (col >= 6)
            return 9;
    }
    return 0;
}

int count(int* array, int value){ 
    int count = 0; 
    for (unsigned long i = 0; i < DIM; i++){ 
        if (array[i] == value) 
            count++; 
    } 
    return count;
}

SudokuConfig* readFile(char* file){
    FILE *mapfile = fopen(file, "r");

    //Construct our holding array
    int* values = (int*)calloc(DIM*DIM, sizeof(int));
    int** array = (int**)malloc(DIM*sizeof(int*));
    for (int i=0; i<DIM; ++i)
        array[i] = values + i*DIM;

    char charbuffer[3];
    /* Reading file into array */
    for (int row = 0; row < DIM; row++) {
        for (int col = 0; col < DIM; col++) {
            if (fgets(charbuffer, 3, mapfile))
                array[row][col] = atoi(charbuffer);
        }
    }
    fclose(mapfile); //close our file ASAP
    
    SudokuConfig* root = new SudokuConfig(array);
    
    //free up the allocated array
    free(values);
    free(array);

    return root;
}

int** createArray(){
    int* values = (int*)calloc(DIM*DIM, sizeof(int));
    int** array = (int**)malloc(DIM*sizeof(int*));
    for (int i=0; i<DIM; ++i)
        array[i] = values + i*DIM;
    return array;
}



int main(int argc, char** argv){
    // if (argc != 2) {
    //     std::cout << "Error: no file specified."<< '\n' <<
    //     "Usage: sudoku nameofboard.txt" << std::endl;
    //     exit(-1);
    // }

    SudokuConfig* root = readFile("/home/kate/git/sudoku/su0.txt"); //readFile(argv[1]);
    // std::cout << "Root:" << '\n' << *root << std::endl;
    // int num = 0;
    // SudokuConfig** suc = (SudokuConfig**)calloc(DIM, sizeof(SudokuConfig*));
    // root->getSuccessors(suc, &num);
    // for (int i = 0; i < num; i++)
    //     std::cout << **(suc + i);
    SudokuConfig* solution = root->solve(false);
    if (solution != NULL)
        std::cout << *solution << std::endl;
    //free(suc);
    delete root;
}
