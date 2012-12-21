"""
File: skyscraper.py (Student version)
Author: Sean Strout <sps@cs.rit.edu>
Author: <<< YOUR NAME HERE >>>
Purpose:  A backtracking solver for the Skyscraper puzzle.
Language: Python 3
"""

from copy import deepcopy
from sys import argv
import collections
import time
import os

class SudokuConfig:
    """
    A class that represents a sudoku configuration.
        board - square grid of values 
            (list of list of int's, size DIM*DIM)
    """
    __slots__ = ('DIM', 'board')
    
    """The empty cell value"""
    EMPTY = 0   # can be referenced anywhere as: SudokuConfig.EMPTY
    
    def __init__(self,initial):
        """
        Constructor.
        """
        self.DIM = 9
        self.board = initial
        # YOUR ARE FREE TO ADD ADDITIONAL FUNCTIONALITY HERE...
                    
    def __str__(self):
        """
        Return a string representation of the config.
        """
        
        # top row
        result = '  '
        result = '\n ' + '-' * (self.DIM*2+5) + '\n'
            
        # board rows
        for row in range(self.DIM):
            if row is 3 or row is 6:
                result += '|' + '-' * (self.DIM*2+5) + '|' + '\n'
                # result += '|-------+-------+-------|\n'
            result += '| '
            for col in range(self.DIM):
                if col is 3 or col is 6:
                    result += '| '
                if self.board[row][col] == SudokuConfig.EMPTY:
                    result += '.'
                else:
                    result += str(str(self.board[row][col]))
                if col != self.DIM-1: result += ' '
            result += ' |' + '\n'
            
        # bottom row
        result += ' ' + '-' * (self.DIM*2+5) + '\n'
        result += '  '
        result += '\n'
                  
        return result
    
    def values(self,direction,index):
        if direction == "r":
            temp = []
            for item in self.board[index]:
                temp.append(item)
            return temp
        elif direction == "c":
            temp = []
            for row in self.board:
                temp.append(row[index])
            return temp
        elif direction == "s":
            temp = []
            if index == 1:
                for r in range(3):
                    for c in range(3):
                        temp.append(self.board[r][c])
            elif index == 2:
                for r in range(3):
                    for c in range(3,6):
                        temp.append(self.board[r][c])
            elif index == 3:
                for r in range(3):
                    for c in range(6,9):
                        temp.append(self.board[r][c])
            elif index == 4:
                for r in range(3,6):
                    for c in range(3):
                        temp.append(self.board[r][c])
            elif index == 5:
                for r in range(3,6):    
                    for c in range(3,6):
                        temp.append(self.board[r][c])
            elif index == 6:
                for r in range(3,6):    
                    for c in range(6,9):
                        temp.append(self.board[r][c])       
            elif index == 7:
                for r in range(6,9):
                    for c in range(3):
                        temp.append(self.board[r][c])
            elif index == 8:
                for r in range(6,9):    
                    for c in range(3,6):
                        temp.append(self.board[r][c])
            elif index == 9:
                for r in range(6,9):    
                    for c in range(6,9):
                        temp.append(self.board[r][c])         
            return temp

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
    DIM = 9
    board = []
    for _ in range(DIM):
        line = [int(val) for val in f.readline().split()]
        board.append(line)
    f.close()
    return SudokuConfig(board)
    
def remove_values_from_list(the_list, val):
   return [value for value in the_list if value != val]

def isGoal(config):
    """
    Checks whether a config is a goal or not
        config: The config (SkyscraperConfig)
    Returns: True if config is a goal, False otherwise
    """
    
    # YOUR IMPLEMENTATION REPLACES THIS
    #for look in range(config.DIM):
    for index in range(config.DIM):
        if not config.values('r',index).count(0) is 0:
            return False
        if not isValid(config):
            return False
    return True


def isValid(config):
    """
    Checks the config to see if it is valid
        config: The config (SkyscraperConfig)
    Returns: True if the config is valid, False otherwise
    """
    
    # YOUR IMPLEMENTATION REPLACES THIS
    #First check if rows and columns contain a value only once
    for index in range(config.DIM):
        if isValidRep(config,index,'r') is False or isValidRep(config,index,'c') is False or isValidRep(config,index,'s') is False:
            return False
    return True

def isValidRep(config,index,direction):
    #print("Testing index :" + index)
    if direction is 'r':
        l = config.values('r',index)
    elif direction is 'c':
        l = config.values('c',index)
    elif direction is 's':
        l = config.values('s',index)   
    for value in range(1,config.DIM+1):
        if l.count(value) > 1:
            #print("Fails at row " + str(row))
            return False
    return True

def getQuadrant(row,col):
    if row < 3:
        if col < 3:
            return 1
        if 3 <= col < 6:
            return 2
        if col >= 6:
            return 3
    elif 3 <= row < 6:
        if col < 3:
            return 4
        if 3 <= col < 6:
            return 5
        if col >= 6:
            return 6
    elif row >= 6:
        if col < 3:
            return 7
        if 3 <= col < 6:
            return 8
        if col >= 6:
            return 9

def getSuccessors(config):
    suc = []
    for row in range(config.DIM):
        usedRowVals = remove_values_from_list(config.values('r',row),0)
        for col in range(config.DIM):
            usedColVals = remove_values_from_list(config.values('c',col),0)
            usedSubVals = remove_values_from_list(config.values('s',getQuadrant(row,col)),0)
            if config.board[row][col] == 0:
                for i in range(1,config.DIM+1):
                    if usedRowVals.count(i) is 0 and usedColVals.count(i) is 0 and usedSubVals.count(i) is 0: #if we already used that value, don't reuse it
                        if i == config.board[row][col]: pass
                        newConfig = deepcopy(config)
                        newConfig.board[row][col] = i
                        suc.append(newConfig)
                return suc
 
    return []

def solve(config, debug):
    """
    Generic backtracking solver.
        config: the current config (SkyscraperConfig)
        debug: print debug output? (Bool)
    Returns:  A config (SkyscraperConfig), if valid, None otherwise
    """
    
    if isGoal(config):
        return config
    else:
        if debug: print('Current:\n' + str(config))
        for successor in getSuccessors(config):
            if isValid(successor):
                if debug: print('Valid Successor:\n' + str(successor))
                solution = solve(successor, debug)
                if solution != None:
                    return solution
    

def main():
    """
    The main program.
        Usage: python3 skyscraper.py [filename debug]
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
        print("Usage: python3 skyscraper.py [filename debug]")
        print("optional command line arguments:" )
        print("  filename: The name of the board file")
        print("  debug: True or False for debug output")
        return -1
        
    # read and display the initial board
    if (os.name == 'nt'):
        time.clock()
    else:
        startTime = time.clock()

    initConfig = readBoard(filename)
    print('Initial Config:\n' + str(initConfig))
    
    # solve the puzzle
    solution = solve(initConfig,debug)
    
    if (os.name == 'nt'):
        print("Time elapsed:",time.clock(),"seconds")
    else:
        print("Time elapsed:",time.clock() - startTime,"seconds")
    pass
    
    # display the solution, if one exists
    if solution != None:
        print('Solution:\n' + str(solution))
    else:
        print('No solution.')
    
if __name__ == '__main__':
    main()
