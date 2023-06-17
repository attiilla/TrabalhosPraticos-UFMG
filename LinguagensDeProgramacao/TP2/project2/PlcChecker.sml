exception EmptySeq
exception UnknownType
exception NotEqTypes
exception WrongRetType
exception DiffBrTypes
exception IfCondNotBool
exception NoMatchResults
exception MatchResTypeDiff
exception MatchCondTypesDiff
exception CallTypeMisM
exception NotFunc
exception ListOutOfRange
exception OpNonList


(*Regras para implementar: *)
(*1,7,8,9,10,11,13,14,15,16,17,18,19,20,21,22,23,24,25,26*)



fun teval (e: expr) (env: plcVal env) :  plcType =
  case e of
     ConB(_) => BoolT (*Regra 2*)
    |ConI(_) => IntT (*Regras 3, 4*) 
    |List nil => ListT nil (*Regra 5*)
    |List(l) =>             (*Regra 6*)
      let
        fun inverse_map fun_list e = 
          case fun_list of
           nil => nil
          |(h::t) => (h e)::(inverse_map t e)
      in 
        ListT(inverse_map (map teval l) env)
      end
    |If(c,a,b) => if (teval c env)<>BoolT then (*Regra 12*)
        raise IfCondNotBool 
      else
        if (teval a env)<>(teval b env) then
          raise DiffBrTypes 
        else
            teval a env
    |Prim1() => 
    |_ => IntT
