(* Infrastructure to run the Plc interpreter*)

CM.make("$/basis.cm");
CM.make("$/ml-yacc-lib.cm");

use "Environ.sml";
use "Absyn.sml";
use "PlcParserAux.sml";
use "PlcParser.yacc.sig";
use "PlcParser.yacc.sml";
use "PlcLexer.lex.sml";
use "Parse.sml";

Control.Print.printLength := 1000;
Control.Print.printDepth  := 1000;
Control.Print.stringDepth := 1000;

open PlcFrontEnd;
use "PlcChecker.sml";
use "PlcInterp.sml";
use "Plc.sml";
use "testCases.sml";
(*use "ParserReference.sml";*)

val interpFile = TextIO.openAppend "Plc-Output";
val caseIdx = ref 1

fun writeResult r = 
	let
		val res = run r
		val idx = !caseIdx
	in
		TextIO.output(interpFile, (Int.toString idx ^ ". " ^ res ^ "\n")); caseIdx := !caseIdx + 1
	end;

(* Test interpreter *)
map (fn x => writeResult (#2(x))) cases;


TextIO.closeOut interpFile;

OS.Process.exit(OS.Process.success);
