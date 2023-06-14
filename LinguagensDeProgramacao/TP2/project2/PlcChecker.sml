(*
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
*)

(*Regras par implementar: *)
(*1,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26*)

(*Exceções a adicionar: *)
(*12*)
fun teval (e: expr) (env: plcVal env) : plcType =
    case e of
          ConB _ => BoolT (*Regra 2*)
        | ConI _ => IntT (*Regras 3, 4*) 
        | List nil => ListT nil (*Regra 5*) 
        | If(c,a,b) => if teval(c)=BoolT andalso teval(a)=teval(b) then
            teval(b) else (*Apagar e adicionar exceção*) BoolT(*Regra 12*)
