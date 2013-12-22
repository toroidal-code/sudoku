module Array = struct
  include Array
  let double_map f arr = 
    Array.map (fun inside -> Array.map f inside) arr

  let count item list = 
    let rec aux n acc = 
      if n = (Array.length list) then acc
      else if (Array.get list n) = item then aux (n + 1) (acc + 1)
      else aux (n + 1) acc
    in (aux 0 0)
end;;


let dim = 9;;

let range i j = 
    let rec aux n acc =
      if n < i then acc 
      else aux (n-1) (n :: acc)
    in (aux (j-1) []) ;;

module IntSet = Set.Make (struct
    let compare = Pervasives.compare
    type t = int
  end);;

module IntSet = struct
  include IntSet
  let of_list list =
    let s = IntSet.empty in
    List.map (fun e -> IntSet.add e s) list
  let of_array arr =
    let s = IntSet.empty in
    Array.map (fun e -> IntSet.add e s) list
  let to_list set = IntSet.elements set
end;;

class cell initial =
  object
    val mutable value : int = initial;
    val mutable possible : int array = [||];
    method set_value new_val = value <- new_val;
    method get_value = value;
    method set_possible poss = possible <- poss;
    method get_possible = possible;
    method print = print_int value;
  end;;

class sudoku initial =
  let empty = 0 in
  object (self)
    val mutable board : cell array array = 
      Array.double_map (fun (i :int) -> new cell i) initial;
      
    method values (direction : int) (index : int) = 
      let cell_to_value (c : cell) = c#get_value in
      if direction = 0 then Array.map cell_to_value (Array.get board index)
      else if direction = 1 then
        Array.map (fun internal  -> cell_to_value (Array.get internal index)) board
      else if direction = 2 then
        let grab row col = 
          Array.of_list (List.concat (List.map (fun r -> List.map (fun c -> cell_to_value (board.(r).(c)) ) col) row)) in
        if index = 0 then grab [0;1;2;] [0;1;2;]
        else if index = 1 then grab [3;4;5;] [0;1;2;]
        else if index = 2 then grab [6;7;8;] [0;1;2;]
        else if index = 3 then grab [0;1;2;] [3;4;5;]
        else if index = 4 then grab [3;4;5;] [3;4;5;]
        else if index = 5 then grab [6;7;8;] [3;4;5;]
        else if index = 6 then grab [0;1;2;] [6;7;8;]
        else if index = 7 then grab [3;4;5;] [6;7;8;]
        else if index = 8 then grab [6;7;8;] [6;7;8;]
        else [||]
      else [||];
    (* method is_valid = List.for_all (fun e -> true && e) (List.concat (List.concat (List.map (fun idx ->  *)
    (*     List.map (fun direc ->  *)
    (*         let vals = (self#values direc idx) in *)
    (*         List.map (fun num ->  *)
    (*             if (Array.count num vals) > 1 then false  *)
    (*             else true) (range 1 (dim + 1)))  *)
    (*       (range 0 3))  *)
    (*     (range 0 dim)))); *)
    (* method is_goal = (List.for_all (fun e -> true && e)  *)
    (*                     (Array.to_list (Array.map (fun row ->  *)
    (*                          if (Array.count 0 (Array.map (fun col -> col#get_value) row)) != 0 then false  *)
    (*                          else true)  *)
    (*                          board))) && self#is_valid; *)
    method is_valid = List.for_all (fun e -> true && e) (List.concat (List.concat ( 
        (range 0 dim) 
        |> List.map (fun idx -> 
            (range 0 3)
            |> List.map (fun direc -> 
                let vals = (self#values direc idx) in
                (range 1 (dim + 1))
                |> List.map (fun num ->
                     (Array.count num vals) <= 1))))))
            

    method is_goal = (List.for_all (fun e -> true && e) (Array.to_list
                        (board 
                         |> Array.map (fun row -> 
                             (row 
                              |> Array.map (fun col -> col#get_value) 
                              |> Array.count 0) = 0 )))) 
                     && self#is_valid;
    (* method gen_poss =  *)
    (*   (range 0 dim) *)
    (*   |> List.map (fun row ->  *)
    (*       (range 0 dim) *)
    (*       |> List.map (fun col ->  *)
    (*           st =  *)
  end;;









