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
(*Caso as regras estejam fora de ordem, é porque a*)
(*8,9,10,11,13,16,17,18,19,20,21,22,23,24,25,26*)
(*revisar: 16, exception 18, 19*)



 let 
        val empty_sequence : expr list = []
      in
        if a=empty_sequence
          then
            let
              
            in
              unity_type
            end
          else
var x = e1
Something(Var("x"),e1)

fun teval (e: expr) (env: plcType env) :  plcType =
  let
    val empty_sequence : expr list = [];
    val unity_type : plcType list = []
  in
    case e of
      Var(a) => lookup env a (*Regra 1*)
      |ConB(_) => BoolT (*Regra 2*)
      |ConI(_) => IntT (*Regras 3, 4*) 
      |List(l) => if l=empty_sequence
        then                  (*Regra 5*)
          ListT(unity_type)
        else                  (*Regra 6*)      
          let
            fun inverse_map fun_list e = 
              case fun_list of
              nil => nil
              |(h::t) => (h e)::(inverse_map t e)
          in 
            ListT(inverse_map (map teval l) env)
          end
      |ESeq(ListT(l)) => ListT(l)    (*Regra 7*)
      |Prim2(";",Prim2("=",Var(s),e1),e2) => teval e2 ((s,(teval e1 env))::env)     (*Regra 8*)
      (*|Letrec(strfun, tp1, strarg, tp2, e1, e2) => *)
      |
      |If(c,a,b) => if (teval c env)<>BoolT (*Regra 12*)
        then 
          raise IfCondNotBool 
        else
          if (teval a env)<>(teval b env) then
            raise DiffBrTypes 
          else
              teval a env
      (*|Prim1(opr,e) => 
        let 
          val plcT = teval e env
        in
          case (opr,plcT) of
            ("!",BoolT) => BoolT (*Regra 14*)
            |("-",t) => t         (*Regra 15*)
            |("hd",SeqT(t)) => if t<>nil (*Regra 16?*)
              then
                t
              else
                raise EmptySeq
            |("tl",SeqT(t)) => if t<>nil (*regra 17*)
              then
                SeqT(t)
              else
                raise EmptySeq
            |()
            |("ise",t) => if t=ListT(_) (*Regra 18*)
              then
                BoolT
              else
                raise OpNonList(*????*)
            |("print",t) => ListT nil (*Regra 19?*)
            |(_,_) => IntT
        end*)
      |_ => IntT
      
  end