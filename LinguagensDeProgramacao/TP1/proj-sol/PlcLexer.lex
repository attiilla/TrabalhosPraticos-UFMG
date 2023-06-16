(* Plc Lexer *)

(* User declarations *)

open Tokens
type pos = int
type slvalue = Tokens.svalue
type ('a,'b) token = ('a,'b) Tokens.token
type lexresult = (slvalue, pos)token

(* A function to print a message error on the screen. *)
val error = fn x => TextIO.output(TextIO.stdOut, x ^ "\n")
val lineNumber = ref 0

(* Get the current line being read. *)
fun getLineAsString() =
    let
        val lineNum = !lineNumber
    in
        Int.toString lineNum
    end

(* Define what to do when the end of the file is reached. *)
fun eof () = Tokens.EOF(0,0)

(* Initialize the lexer. *)
fun init() = ()
%%
%header (functor PlcLexerFun(structure Tokens: PlcParser_TOKENS));
digit=[0-9];
ws = [\ \t];
string = [a-zA-Z_][a-zA-Z_0-9]*;
%%

\n => (lineNumber := (!lineNumber) + 1; lex());
{ws}+ => (lex());
{digit}+ => (Tokens.NAT(valOf (Int.fromString yytext), !lineNumber, !lineNumber));
"Bool" => (Tokens.BOOL(!lineNumber, !lineNumber));
"else" => (Tokens.ELSE(!lineNumber, !lineNumber));
"end" => (Tokens.END(!lineNumber, !lineNumber));
"false" => (Tokens.FALSE(!lineNumber, !lineNumber));
"fn" => (Tokens.FN(!lineNumber, !lineNumber));
"fun" => (Tokens.FUN(!lineNumber, !lineNumber));
"hd" => (Tokens.HD(!lineNumber, !lineNumber));
"if" => (Tokens.IF(!lineNumber, !lineNumber));
"Int" => (Tokens.INT(!lineNumber, !lineNumber));
"ise" => (Tokens.ISE(!lineNumber, !lineNumber));
"match" => (Tokens.MATCH(!lineNumber, !lineNumber));
"Nil" => (Tokens.NIL(!lineNumber, !lineNumber));
"print" => (Tokens.PRINT(!lineNumber, !lineNumber));
"rec" => (Tokens.REC(!lineNumber, !lineNumber));
"then" => (Tokens.THEN(!lineNumber, !lineNumber));
"tl" => (Tokens.TL(!lineNumber, !lineNumber));
"true" => (Tokens.TRUE(!lineNumber, !lineNumber));
"var" => (Tokens.VAR(!lineNumber, !lineNumber));
"with" => (Tokens.WITH(!lineNumber, !lineNumber));
"_" => (Tokens.UNDERSCORE(!lineNumber, !lineNumber));
{string} => (Tokens.NAME(yytext, !lineNumber, !lineNumber));
";" => (Tokens.SEMICOLON(!lineNumber, !lineNumber));
"->" => (Tokens.ARROW(!lineNumber, !lineNumber));
"=>" => (Tokens.FNARROW(!lineNumber, !lineNumber));
"&&" => (Tokens.AND(!lineNumber, !lineNumber));
"=" => (Tokens.EQUAL(!lineNumber, !lineNumber));
"!=" => (Tokens.NOTEQUAL(!lineNumber, !lineNumber));
"<" => (Tokens.LT(!lineNumber, !lineNumber));
"<=" => (Tokens.LTE(!lineNumber, !lineNumber));
"::" => (Tokens.DOUBLECOLON(!lineNumber, !lineNumber));
"+" => (Tokens.PLUS(!lineNumber, !lineNumber));
"-" => (Tokens.MINUS(!lineNumber, !lineNumber));
"*" => (Tokens.TIMES(!lineNumber, !lineNumber));
"/" => (Tokens.DIV(!lineNumber, !lineNumber));
"!" => (Tokens.NOT(!lineNumber, !lineNumber));
"[" => (Tokens.LBRACKET(!lineNumber, !lineNumber));
"]" => (Tokens.RBRACKET(!lineNumber, !lineNumber));
"(" => (Tokens.LPAR(!lineNumber, !lineNumber));
")" => (Tokens.RPAR(!lineNumber, !lineNumber));
"{" => (Tokens.LBRACES(!lineNumber, !lineNumber));
"}" => (Tokens.RBRACES(!lineNumber, !lineNumber));
":" => (Tokens.COLON(!lineNumber, !lineNumber));
"," => (Tokens.COMMA(!lineNumber, !lineNumber));
"|" => (Tokens.PIPE(!lineNumber, !lineNumber));
. => (error ("ignoring bad character "^yytext); lex());