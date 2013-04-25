#Sudoku
##What it is
This is a simple sudoku solver implemented using a backtracking algorithm. It is not (terribly) fast, but is effiecient.
I use this as a benchmark of new systems, since I generally know how well python and sbcl perform in relation to one another.

##New Algorithm
sudoku2 is a new solving method, that is a hybrid generation/backtracking algorithm. Python can now solve `su9.txt` in ~0.8 seconds.
  
##To run:
Either do `python3 sudoku.py [name of board] [debug bool (True or False)]`
###OR
Open sbcl, `(load "sudoku.lisp")` and `(time (solve (read-file-to-array "su9.txt") NIL))`. `NIL` is replaceable with `t` if you want debug output.

##To Do
~~I'm working on a C implementation, though I haven't touched C in years, so who knows how that's going to work. We'll see what turns up.~~
####Update
There is now a C++ implementation, using the old algorithm.

##License
This code is hereby released under the MIT License.
