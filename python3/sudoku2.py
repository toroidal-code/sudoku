#! /usr/bin/python3
from sys import argv
from copy import deepcopy
import time
import os

DIM = 9


class Cell:
    __slots__ = ('value', 'possible')

    def __init__(self, initial):
        """
        Constructor
        """
        self.value = initial
        self.possible = []  # [0,0,0,0,0,0,0,0,0]

    def __str__(self):
        """
        Return a string representation of the cell
        """
        return str(self.value)


class Sudoku:
    __slots__ = ('board')

    EMPTY = 0

    def __init__(self, initial):
        self.board = [] * DIM
        for row in initial:
            self.board.append([Cell(value) for value in row])

    def __str__(self):
        """
        Return a string representation of the config.
        """

        # top row
        result = '  '
        result = '\n ' + '-' * (DIM * 2 + 5) + '\n'

        # board rows
        for row in range(DIM):
            if row is 3 or row is 6:
                result += '|' + '-' * (DIM * 2 + 5) + '|' + '\n'
                # result += '|-------+-------+-------|\n'
            result += '| '
            for col in range(DIM):
                if col is 3 or col is 6:
                    result += '| '
                if self.board[row][col].value == 0:
                    result += '.'
                else:
                    result += str(self.board[row][col])
                if col != DIM - 1:
                    result += ' '
            result += ' |' + '\n'

        # bottom row
        result += ' ' + '-' * (DIM * 2 + 5) + '\n'
        result += '  '
        result += '\n'
        return result

    def values(self, direction, index):
        if direction == 0:
            temp = []
            for item in self.board[index]:
                temp.append(item.value)
            return temp
        elif direction == 1:
            temp = []
            for row in self.board:
                temp.append(row[index].value)
            return temp
        elif direction == 2:
            temp = []
            if index == 0:
                for r in range(3):
                    for c in range(3):
                        temp.append(self.board[r][c].value)
            elif index == 1:
                for r in range(3):
                    for c in range(3, 6):
                        temp.append(self.board[r][c].value)
            elif index == 2:
                for r in range(3):
                    for c in range(6, 9):
                        temp.append(self.board[r][c].value)
            elif index == 3:
                for r in range(3, 6):
                    for c in range(3):
                        temp.append(self.board[r][c].value)
            elif index == 4:
                for r in range(3, 6):
                    for c in range(3, 6):
                        temp.append(self.board[r][c].value)
            elif index == 5:
                for r in range(3, 6):
                    for c in range(6, 9):
                        temp.append(self.board[r][c].value)
            elif index == 6:
                for r in range(6, 9):
                    for c in range(3):
                        temp.append(self.board[r][c].value)
            elif index == 7:
                for r in range(6, 9):
                    for c in range(3, 6):
                        temp.append(self.board[r][c].value)
            elif index == 8:
                for r in range(6, 9):
                    for c in range(6, 9):
                        temp.append(self.board[r][c].value)
            return temp

    def isValid(self):
        for idx in range(DIM):
            for direc in range(3):
                vals = self.values(direc, idx)
                for num in range(1, DIM + 1):
                    if vals.count(num) > 1:
                        return False
        return True

    def isGoal(self):
        """
        Checks whether a config is a goal or not
            config: The config (SkyscraperConfig)
        Returns: True if config is a goal, False otherwise
        """
        for index in range(DIM):
            if not self.values(0, index).count(0) is 0:
                return False
        if not self.isValid():
            return False
        return True

    def printPoss(self):
        for row in self.board:
            print([cell.possible for cell in row])

    def genPoss(self):
        for row in range(DIM):
            for col in range(DIM):
                st = set(self.values(0, row)) | \
                     set(self.values(1, col)) | \
                     set(self.values(2, getQuadrant(row, col)))
                if self.board[row][col].value is 0:
                    self.board[row][col].possible = list(set(range(1,10)) - st)
                else:
                    self.board[row][col].possible = []

    def process(self):
        self.genPoss()
        while(self.getMinPoss() is 1):
            for row in range(DIM):
                for col in range(DIM):
                    poss = self.board[row][col].possible
                    if len(poss) is 1:
                        self.board[row][col].value = poss[0]
            self.genPoss()

    def getMinPoss(self):
        l = DIM
        for row in range(DIM):
            for col in range(DIM):
                poss = self.board[row][col].possible
                if len(poss) < l and \
                   len(poss) is not 0:
                    l = len(poss)
        return l

    def getSuccessors(self):
        suc = []
        for row in range(DIM):
            for col in range(DIM):
                if self.board[row][col].value is 0 and len(self.board[row][col].possible) is self.getMinPoss():
                    for i in self.board[row][col].possible:
                        newConfig = deepcopy(self)
                        newConfig.board[row][col].value = i
                        newConfig.process()
                        suc.append(newConfig)
                    return suc

        return []

    def solve(self, debug):
        """
        Generic backtracking solver.
            config: the current config (SkyscraperConfig)
            debug: print debug output? (Bool)
        Returns:  A config (SkyscraperConfig), if valid, None otherwise
        """
        if self.isGoal():
            return self
        else:
            if debug:
                print('Current:\n' + str(self))
            for successor in self.getSuccessors():
                if self.isValid():
                    if debug:
                        print('Valid Successor:\n' + str(successor))
                    solution = successor.solve(debug)
                    if solution is not None:
                        return solution


def getQuadrant(row, col):
    if row < 3:
        if col < 3:
            return 0
        if 3 <= col < 6:
            return 1
        if col >= 6:
            return 2
    elif 3 <= row < 6:
        if col < 3:
            return 3
        if 3 <= col < 6:
            return 4
        if col >= 6:
            return 5
    elif row >= 6:
        if col < 3:
            return 6
        if 3 <= col < 6:
            return 7
        if col >= 6:
            return 8


def readBoard(filename):
    """
    Read the board file.  It is organized as follows:
        row 1 values    # 0 for empty, (1-DIM) otherwise
        row 2 values    # 0 for empty, (1-DIM) otherwise
        ...

        filename: The file name (string)
    Returns: A config (SkyscraperConfig) containing the board info from file
    """

    f = open(filename)
    board = []
    for _ in range(DIM):
        line = [int(val) for val in f.readline().split()]
        board.append(line)
    f.close()
    return Sudoku(board)


def main():
    """
    The main program.
        Usage: python3 sudoku.py [filename debug]
            filename: The name of the board file
            debug: True or False for debug output
    """

    # if no command line arguments specified, prompt for the filename
    # and set debug output to False
    if len(argv) == 1:
        filename = input('Enter board file: ')
        debug = eval(input("Debug output (True or False): "))
    # otherwise, use the command line arguments
    elif len(argv) == 3:
        filename = argv[1]
        debug = eval(argv[2])
    # incorrect number of command line arguments
    else:
        print("Usage: python3 sudoku.py [filename debug]")
        print("optional command line arguments:")
        print("  filename: The name of the board file")
        print("  debug: True or False for debug output")
        return -1

    # read and display the initial board
    if os.name == 'nt':
        time.clock()
    else:
        startTime = time.clock()

    initConfig = readBoard(filename)
    print('Initial Config:\n' + str(initConfig))
    initConfig.process()
    print('Processed:\n' + str(initConfig))

    # solve the puzzle
    solution = initConfig.solve(debug)

    if os.name == 'nt':
        print("Time elapsed:", time.clock(), "seconds")
    else:
        print("Time elapsed:", time.clock() - startTime, "seconds")

    # display the solution, if one exists
    if solution is not None:
        print('Solution:\n' + str(solution))
    else:
        print('No solution.')


main()
