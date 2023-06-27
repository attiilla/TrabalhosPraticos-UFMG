(* Plc interpreter main file *)

fun run (e: expr): string =
    let val tev = teval e [] val ev = eval e [] in
        (val2string ev) ^ " : " ^ (type2string tev)
    end 

handle SymbolNotFound => "Expression contains undefined variables."
        | UnknownType => "Plc Checker: Wrong expression or operator."
        | NotEqTypes => "Plc Checker: Different types on equality operator."
        | WrongRetType => "Plc Checker: Function returns a different type than the defined one."
        | DiffBrTypes => "Plc Checker: The if branches must have the same type."
        | IfCondNotBool => "Plc Checker: The if condition must be a boolean value."
        | NoMatchResults => "Plc Checker: The Match have no cases."
        | MatchResTypeDiff => "Plc Checker: All cases of the Match must be of the same type."
        | MatchCondTypesDiff => "Plc Checker: The expression don't agree with the Match type."
        | CallTypeMisM => "Plc Checker: Wrong type in function call."
        | NotFunc => "Plc Checker: Attempt to call a non function name."
        | ListOutOfRange => "Plc Checker: Index out of list range."
        | OpNonList => "Plc Checker: Not possible to access index of a non list variable."
        | Impossible => "Plc Interp: Undefined operator used or attempt to use operator with wrong expressions."
        | HDEmptySeq => "Plc Interp: Not possible to access the head of an empty sequence."
        | TLEmptySeq => "Plc Interp: Not possible to access the tail of an empty sequence."
        | ValueNotFoundInMatch => "Plc Interp: Failed to match given pattern with the defined ones."
        | NotAFunc => "Plc Interp: Attempt to call a non function name."
        | ListOutOfRange2 => "Plc Interp: Index out of list range."
        | OpNonList2 => "Plc Interp: Not possible to access index of a non list variable."
        | EvalError => "Plc Interp: Expression can't be evaluated."
