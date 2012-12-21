(defun parse-line (line)
  (with-input-from-string (s line)
			  (loop for num = (read s nil nil)
				while num
				collect num)))

(defun read-file (file-name)
  (with-open-file (stream file-name)
		  (loop while (peek-char nil stream nil nil)
			collect (parse-line (read-line stream nil)))))

(defun flatten (l)
  (cond ((null l) nil)
        ((atom l) (list l))
        (t (loop for a in l appending (flatten a)))))

(defun list-to-2d-array (list)
  (make-array (list (length list)
		    (length (first list)))
	      :initial-contents list))

(defun 2d-array-to-list (array)
  (loop for i below (array-dimension array 0)
	collect (loop for j below (array-dimension array 1)
		      collect (aref array i j))))

(defun read-file-to-array (file-name)
  (list-to-2d-array (read-file file-name)))

(defun get-row (array row)
  (let ((dim (first (array-dimensions array))))
    (mapcar #'(lambda (x) (row-major-aref array x)) (loop for i from (* dim row) to (+ (- dim 1) (* dim row)) collect i))))

(defun get-row-array (array row)
  (coerce (get-row array row) 'vector))

(defun get-col (array col)
  (let ((dim (first (array-dimensions array))))
    (mapcar #'(lambda (x) (row-major-aref array x)) (mapcar #'(lambda (x) (+ (* dim x) col)) (loop for i below dim collect i)))))

(defun get-col-array (array col)
  (coerce (get-col array col) 'vector))

(defun get-subsq (array sq) 
  (cond
   ((= sq 1)
    (loop for r below 3 collect (loop for c below 3 collect (aref array r c))))
   ((= sq 2)
    (loop for r below 3 collect (loop for c from 3 below 6 collect (aref array r c))))
   ((= sq 3)
    (loop for r below 3 collect (loop for c from 6 below 9 collect (aref array r c))))
   ((= sq 4)
    (loop for r from 3 below 6 collect (loop for c below 3 collect (aref array r c))))
   ((= sq 5)
    (loop for r from 3 below 6 collect (loop for c from 3 below 6 collect (aref array r c))))
   ((= sq 6)
    (loop for r from 3 below 6 collect (loop for c from 6 below 9 collect (aref array r c))))
   ((= sq 7)
    (loop for r from 6 below 9 collect (loop for c below 3 collect (aref array r c))))
   ((= sq 8)
    (loop for r from 6 below 9 collect (loop for c from 3 below 6 collect (aref array r c))))
   ((= sq 9)
    (loop for r from 6 below 9 collect (loop for c from 6 below 9 collect (aref array r c))))))

(defun get-subsq-flat (array sq)
  (flatten (get-subsq array sq)))

(defun get-subsq-array (array sq)
  (list-to-2d-array (get-subsq array sq)))

(defun is-goal (array) 
  (loop for i below 9 do
	(cond
	 ((/= (count 0 (get-row array i)) 0)
	  (return-from is-goal NIL))))
  (return-from is-goal t))

(defun get-subsq-index (row col)
  (cond
   ((< row 3)
    (cond
     ((< col 3)
      (return-from get-subsq-index 1))
     ((and (<= 3 col) (< col 6))
      (return-from get-subsq-index 2))
     ((>= col 6)
      (return-from get-subsq-index 3))))
   ((and (<= 3 row) (< row 6))
    (cond
     ((< col 3)
      (return-from get-subsq-index 4))
     ((and (<= 3 col) (< col 6))
      (return-from get-subsq-index 5))
     ((>= col 6)
      (return-from get-subsq-index 6))))
   ((>= row 6)
    (cond
     ((< col 3)
      (return-from get-subsq-index 7))
     ((and (<= 3 col) (< col 6))
      (return-from get-subsq-index 8))
     ((>= col 6)
      (return-from get-subsq-index 9))))))

(defun copy-array (array &key (element-type (array-element-type array))
                              (fill-pointer (and (array-has-fill-pointer-p array)
                                                 (fill-pointer array)))
                              (adjustable (adjustable-array-p array)))
  "Returns an undisplaced copy of ARRAY, with same fill-pointer and
adjustability (if any) as the original, unless overridden by the keyword
arguments."
 (let* ((dimensions (array-dimensions array))
        (new-array (make-array dimensions
                               :element-type element-type
                               :adjustable adjustable
                               :fill-pointer fill-pointer)))
   (dotimes (i (array-total-size array))
     (setf (row-major-aref new-array i)
           (row-major-aref array i)))
   new-array))

(defun get-successors (array)
  (let ((suc NIL))
    (loop for row below 9 do
	  (let ((used-row-vals (remove 0 (get-row array row))))
	    (loop for col below 9 do
		  (let ((used-col-vals (remove 0 (get-col array col)))
			(used-sub-vals (remove 0 (get-subsq-flat array (get-subsq-index row col)))))
		    (cond
		     ((= 0 (aref array row col))
		      (loop for i from 1 to 9 do
			    ;(print (list row col))
			    ;(print i)
			    ;(print (list used-row-vals used-col-vals used-sub-vals))
			    (cond
			     ((and (= 0 (count i used-row-vals))
				   (= 0 (count i used-col-vals))
				   (= 0 (count i used-sub-vals)))
			      (let ((config (copy-array array)))
				(setf (aref config row col) i)
				;(print config)
				;(print (list config))
				(setf suc (append suc (list config)))
				;(print suc)
				))))
		      (return-from get-successors suc)))))))))

(defun solve (array debug)
  (cond
   ((is-goal array)
    (return-from solve array))
   (t
    (cond (debug (progn (format t "Current:~%") (print-board array))))
    (loop for successor in (get-successors array) do
	  (cond (debug (progn (format t "Valid Successor:~%") (print-board successor))))
	  (let ((solution (solve successor debug)))
	    (cond
	     (solution
	      (return-from solve solution))))))))

(defun print-board (array)
  (loop for row below 9 do
	(cond
	 ((or (= row 3) (= row 6))
	  (format t "-------+-------+-------~%")))
	(loop for col below 9 do
	      (cond
	       ((or (= 3 col) (= 6 col))
		(format t " |")))
	      (format t " ~A" (aref array row col)))
	(format t "~%")))
