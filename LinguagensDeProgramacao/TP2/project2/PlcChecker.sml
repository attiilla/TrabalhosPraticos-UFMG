(* PlcChecker *)

exception EmptySeq
exception UnknownType
exception NotEqTypes
exception WrongRetType
exception DiffBrTypes
exception IfCondNotBool
exception NoMatchResults
exception MatchCondTypesDiff
exception CallTypeMisM
exception NotFunc
exception ListOutOfRange
exception OpNonList

fun getItemType (n:int, l:plcType list) =
   case (n, l) of
          (_, []) => raise ListOutOfRange
        | (1, b) => hd b
        | (a, b) => getItemType (a-1, tl b);


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
      |ESeq(SeqT(l)) => SeqT(l)             (*Regra 7*)
      |Let(s,e1,e2) => teval e2 ((s,(teval e1 env))::env)     (*Regra 8*)
      |Letrec(strfun, arg_tp, strarg, f_tp, e1, e2) =>        (*Regra 9*)
      if (teval e1 ((strfun,FunT(arg_tp,f_tp))::(strarg,arg_tp)::env))=f_tp
        then
          teval e2 ((strfun,FunT(arg_tp,f_tp))::env)
        else
          raise WrongRetType
      |Anon(t,strarg,e1) => (*Regra 10*)
      let
        val t2 = teval e1 ((strarg,t)::env)
      in
        FunT(t,t2)
      end
      |Call(e1,e2) =>       (*Regra 11*)
        let val t1 = teval e1 env val t2 = teval e2 env in
          case (t1) of
             FunT(a,b) => if (a = t2) then b else raise CallTypeMisM
            |_ => raise NotFunc
        end
      |If(c,a,b) => if (teval c env)<>BoolT (*Regra 12*)
        then
          raise IfCondNotBool
        else
          if (teval a env)<>(teval b env) then
            raise DiffBrTypes
          else
              teval a env
      |Match(ex,ls) =>                    (*Regra 13*)
        let
          fun checkListType (l: (expr option*expr) list) : bool =
          if l = nil
            then
              true
            else
              case #1(hd l) of
                 SOME(e) => if (teval ex env)=(teval e env)
                  then
                    checkListType (tl l)
                  else
                    false
                |NONE => true;
          fun checkListReturnType [] = true
          | checkListReturnType (_::[]) = true
          | checkListReturnType ((_, e1)::(a, e2)::t) =
            if (teval e1 env) = (teval e2 env)
              then
                checkListReturnType((a,e2)::t)
              else
                false
        in
          if checkListType ls
            then
              if (ls = []) then raise NoMatchResults else
              teval (#2(hd ls)) env
            else
              raise MatchCondTypesDiff
        end
      |Prim1(opr,e1) =>
        let
          val plcT = teval e1 env
        in
          case (opr,plcT) of
             ("!", BoolT) => BoolT    (*Regra 14*)
            |("-", IntT) => IntT      (*Regra 15*)
            |("hd", SeqT t) => t      (*Regra 16*)
            |("tl", SeqT t) => SeqT t      (*Regra 17*)
            |("ise",SeqT t) => BoolT  (*Regra 18*)
            |("print", t) => ListT(unity_type) (*Regra 19*)
            |(_,_) => raise UnknownType
          end
      |Prim2(opr,e1,e2) =>
        let
          val plcT1 = teval e1 env;
          val plcT2 = teval e2 env
        in
          case(opr,plcT1,plcT2) of
             ("&&",_,_) => if ((teval e1 env)=BoolT) andalso ((teval e2 env)=BoolT) (*Regra 20*)
              then
                BoolT
              else
                raise UnknownType
            |("::",t1,SeqT(t2)) => if t2=t1     (*Regra 21*)
              then
                SeqT(t2)
              else
                raise UnknownType
            |("+",IntT,IntT) => IntT        (*Regra 22.00*)
            |("-",IntT,IntT) => IntT        (*Regra 22.25*)
            |("*",IntT,IntT) => IntT        (*Regra 22.50*)
            |("/",IntT,IntT) => IntT        (*Regra 22.75*)
            |("<",IntT,IntT) => BoolT       (*Regra 23.0*)
            |("<=",IntT,IntT) => BoolT      (*Regra 23.5*)
            |("=",t1,t2) => (               (*Regra 24.0*)
              case (t1,t2) of
                 (FunT(_,_),_) => raise UnknownType
                |(_,FunT(_,_)) => raise UnknownType
                |(_,_) => if t1<>t2
                  then
                    raise NotEqTypes
                  else
                    BoolT)
            |("!=",t1,t2) =>(                (*Regra 24.5*)
              case (t1,t2) of
                 (FunT(_,_),_) => raise UnknownType
                |(_,FunT(_,_)) => raise UnknownType
                |(_,_) => if t1<>t2
                  then
                    raise NotEqTypes
                  else
                    BoolT)
            |(";",_,t2) => t2             (*Regra 26*)
            |(_,_,_) => raise UnknownType
        end
      |Item(i,e1) =>             (*Regra 25*)
        let val v1 = teval e1 env in
           case (v1) of
                  (ListT l) => getItemType (i, l)
                | (_) => raise OpNonList
            end
      |(_) => raise UnknownType
    end
