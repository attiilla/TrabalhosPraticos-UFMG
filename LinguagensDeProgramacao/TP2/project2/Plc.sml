(* Plc interpreter main file *)

fun run (e: expr): string =
    let val tev = teval e [] val ev = eval e [] in
        (val2string ev) ^ " : " ^ (type2string tev)
    end 

handle SymbolNotFound => "Expression contains undefined variables."
        | UnknownType => "Wrong expression or operator."
        | NotEqTypes => "Different types on equality operator."
        | WrongRetType => "Function returns a different type than the defined one."
        | DiffBrTypes => "The if branches must have the same type."
        | IfCondNotBool => "The if condition must be a boolean value."
        | MatchCondTypesDiff => "The Match is not written correctly."
        | CallTypeMisM => "Wrong type in function call."
        | NotFunc => "Attempt to call a non function name."
        | ListOutOfRange => "Index out of list range."
        | OpNonList => "Not possible to access index of a non list variable."
        | Impossible => "Undefined operator used or attempt to use operator with wrong expressions."
        | HDEmptySeq => "Not possible to access the head of an empty sequence."
        | TLEmptySeq => "Not possible to access the tail of an empty sequence."
        | ValueNotFoundInMatch => "Failed to match given pattern with the defined ones."
        | NotAFunc => "Attempt to call a non function name."
        | EvalError => "Expression can't be evaluated."
