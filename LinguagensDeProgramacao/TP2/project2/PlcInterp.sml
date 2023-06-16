(* PlcInterp *)

(*exception Impossible
exception HDEmptySeq
exception TLEmptySeq
exception ValueNotFoundInMatch
exception NotAFunc*)

fun eval (e:expr) (env:plcVal env) : plcVal =
    case e of
          ConI i => IntV i
        | ConB b => BoolV b
        | ESeq (SeqT t) => SeqV[]
        | Var str => lookup env str
        | If (e1, e2, e3) => if (eval e1 env) = BoolV true then eval e2 env else eval e3 env
        | Let (str, e1, e2) => eval e2 ((str, (eval e1 env))::env)
        | Letrec (strfun, tp1, strarg, tp2, e1, e2) => eval e2 ((strfun, Clos(strfun, strarg, e1, env))::env)
        | Prim1 (opr, e1) => let val v = eval e1 env in
            case (opr, v) of
                  ("!", BoolV b) => BoolV (not b)
                | ("-", IntV i) => IntV (~i)
                | ("print", _) => let val s = val2string v in print(s^"\n"); ListV[] end
                | ("ise", SeqV a) => if a = [] then BoolV true else BoolV false
                | ("hd", SeqV a) => if a = [] then IntV 0 (*Substituir por exception*) else hd a
                | ("tl", SeqV a) => if a = [] then IntV 0 (*Substituir por exception*) else SeqV (tl a)
                | (_,_) => IntV 0 (*Substituir por exception*)
            end
        | Prim2 (opr, e1, e2) => let val v1 = eval e1 env val v2 = eval e2 env in
            case (opr, v1, v2) of
                  ("+", IntV a, IntV b) => IntV (a + b)
                | ("-", IntV a, IntV b) => IntV (a - b)
                | ("*", IntV a, IntV b) => IntV (a * b)
                | ("/", IntV a, IntV b) => IntV (a div b)
                | ("&&", BoolV a, BoolV b) => BoolV (a andalso b)
                | ("<", IntV a, IntV b) => BoolV (a < b)
                | ("<=", IntV a, IntV b) => BoolV (a <= b)
                | (";", _, _) => let val first = v1 in let val second = v2 in first; second end end
                | ("::", a, SeqV b) => SeqV (a::b)
                | ("=", a, b) => BoolV (a = b)
                | ("!=", a, b) => BoolV (a <> b)
                | (_,_,_) => IntV 0 (*Substituir por exception*)
            end
        | List ([]) => ListV ([])
        (* List talvez esteja dando problema com List de List: lembrar de fazer testes do tipo *)
        | List (l) => let val v1 = (eval (hd l) env) val v2 = (eval (List (tl l)) env) in
            case (v2) of
                  (ListV ([])) => ListV [v1]
                | (ListV (l1)) => ListV (v1::l1)
                | (_) => IntV 0 (*Substituir por exception*)
            end
        | Item (i, List l) => if i = 1 then eval (hd l) env else eval (Item (i - 1, List (tl l))) env
        | Match (e1, l) => let val v1 = eval e1 env in
                                if isSome (#1 (hd l)) then
                                    if v1 = eval (valOf (#1 (hd l))) env then eval (#2 (hd l)) env
                                    else eval (Match(e1, (tl l))) env
                                else  eval (#2 (hd l)) env
                            end
        | Call (Var str, e1) => let val cl = lookup env str val vl = eval e1 env in
            case (cl) of
                  (Clos (f, par, e, st)) => eval e ((par, vl)::(f, cl)::st)
                | (_) => IntV 0 (*Substituir por exception*)
            end
        | Anon (tp, str, e1) => Clos (" ", str, e1, env)
        | _ => IntV 0; (*Substituir por exception*)
