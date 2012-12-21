#Sudoku
##What it is
This is a simple sudoku solver implemented using a backtracking algorithm. It is not (terribly) fast, but is effiecient. I use this as a benchmark of new systems, since I generally know how well python and sbcl perform in relation to one another.

##For comparison
Python takes 25 seconds on a MacBook 2GHz Core 2 Duo 64bit, while sbcl has a result of:
`Evaluation took:
  1.388 seconds of real time
  1.348433 seconds of total run time (1.258145 user, 0.090288 system)
  [ Run times consist of 0.228 seconds GC time, and 1.121 seconds non-GC time. ]
  97.12% CPU
  2,717,469,105 processor cycles
  35 page faults
  629,127,424 bytes consed`
  
##To run:
Either do `python3 sudoku.py` and then enter `su9.txt` and `False`
###OR
Open sbcl, `(load "sudoku.lisp")` and `(time (solve (read-file-to-array "su9.txt") NIL))`

##To Do
I'm working on a C implementation, though I haven't touched C in years, so who knows how that's going to work. We'll see what turns up.

##License
This code is hereby released under the MIT License.
