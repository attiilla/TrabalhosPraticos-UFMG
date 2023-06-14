%%

%name PlcParser

%pos int

%term BOOL | ELSE | END | FALSE | FN | FUN | HD | IF | INT | ISE |
      MATCH | NIL | PRINT | REC | THEN | TL | TRUE | VAR | WITH | UNDERSCORE | 
      SEMICOLON | ARROW | FNARROW | AND | EQUAL | NOTEQUAL | LT | LTE | DOUBLECOLON | 
      PLUS | MINUS | TIMES | DIV | NOT | LBRACKET | RBRACKET |
      LPAR | RPAR | LBRACES | RBRACES | COLON | COMMA | PIPE | EOF |
      NAME of string | NAT of int

%nonterm Prog of expr |
         Decl of expr |
         Expr of expr |
         AtomicExpr of expr |
         AppExpr of expr |
         Const of expr |
         Comps of expr list |
         MatchExpr of (expr option * expr) list |
         CondExpr of expr option |
         Args of (plcType * string) list |
         Params of (plcType * string) list |
         TypedVar of (plcType * string) |
         Type of plcType |
         AtomicType of plcType |
         Types of plcType list

%right SEMICOLON ARROW
%nonassoc IF
%left ELSE
%left AND
%left EQUAL NOTEQUAL
%left LT LTE
%right DOUBLECOLON
%left PLUS MINUS
%left TIMES DIV
%nonassoc NOT HD TL ISE PRINT
%left LBRACKET

%eop EOF

%noshift EOF

%start Prog

%%

Prog: Expr (Expr) 
    | Decl (Decl)

Decl: VAR NAME EQUAL Expr SEMICOLON Prog (Let(NAME, Expr, Prog))
    | FUN NAME Args EQUAL Expr SEMICOLON Prog (Let(NAME, makeAnon(Args, Expr), Prog))
    | FUN REC NAME Args COLON Type EQUAL Expr SEMICOLON Prog (makeFun(NAME, Args, Type, Expr, Prog))

Expr: AtomicExpr (AtomicExpr)
    | AppExpr (AppExpr)
    | IF Expr THEN Expr ELSE Expr (If(Expr1, Expr2, Expr3))
    | MATCH Expr WITH MatchExpr (Match(Expr, MatchExpr))
    | NOT Expr (Prim1("!", Expr))
    | MINUS Expr (Prim1("-", Expr))
    | HD Expr (Prim1("hd", Expr))
    | TL Expr (Prim1("tl", Expr))
    | ISE Expr (Prim1("ise", Expr))
    | PRINT Expr (Prim1("print", Expr))
    | Expr AND Expr (Prim2("&&", Expr1, Expr2))
    | Expr PLUS Expr (Prim2("+", Expr1, Expr2))
    | Expr MINUS Expr (Prim2("-", Expr1, Expr2))
    | Expr TIMES Expr (Prim2("*", Expr1, Expr2))
    | Expr DIV Expr (Prim2("/", Expr1, Expr2))
    | Expr EQUAL Expr (Prim2("=", Expr1, Expr2))
    | Expr NOTEQUAL Expr (Prim2("!=", Expr1, Expr2))
    | Expr LT Expr (Prim2("<", Expr1, Expr2))
    | Expr LTE Expr (Prim2("<=", Expr1, Expr2))
    | Expr DOUBLECOLON Expr (Prim2("::", Expr1, Expr2))
    | Expr SEMICOLON Expr (Prim2(";", Expr1, Expr2))
    | Expr LBRACKET NAT RBRACKET (Item(NAT, Expr))

AtomicExpr: Const (Const)
    | NAME (Var(NAME))
    | LBRACES Prog RBRACES (Prog)
    | LPAR Expr RPAR (Expr)
    | LPAR Comps RPAR (List(Comps))
    | FN Args FNARROW Expr END (makeAnon(Args, Expr))

AppExpr: AtomicExpr AtomicExpr (Call(AtomicExpr1, AtomicExpr2))
    | AppExpr AtomicExpr (Call(AppExpr, AtomicExpr))

Const: TRUE (ConB(true))
    | FALSE (ConB(false))
    | NAT (ConI(NAT))
    | LPAR RPAR (List [])
    | LPAR Type LBRACKET RBRACKET RPAR (ESeq(Type))

Comps: Expr COMMA Expr ([Expr1, Expr2])
    | Expr COMMA Comps (Expr::Comps)

MatchExpr: END ([])
    | PIPE CondExpr ARROW Expr MatchExpr ((CondExpr, Expr)::MatchExpr)

CondExpr: Expr (SOME(Expr))
    | UNDERSCORE (NONE)

Args: LPAR RPAR ([])
    | LPAR Params RPAR (Params)

Params: TypedVar ([TypedVar])
    | TypedVar COMMA Params (TypedVar::Params)

TypedVar: Type NAME ((Type, NAME))

Type: AtomicType (AtomicType)
    | LPAR Types RPAR (ListT(Types))
    | LBRACKET Type RBRACKET (SeqT(Type))
    | Type ARROW Type (FunT(Type1, Type2))

AtomicType: NIL (ListT([]))
    | BOOL (BoolT)
    | INT (IntT)
    | LPAR Type RPAR (Type)

Types: Type COMMA Type ([Type1, Type2])
    | Type COMMA Types (Type::Types)
