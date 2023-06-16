val cases : (string * expr) list =
    (let
                          val Concr = "fun highAdd(Int x) = fn(Int y) => x + y end; highAdd(3)(4)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 1 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun rec f(Int n):Int = if n <= 0 then 0 else n + f(n-1); f(15)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 2 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fn (Int x) => -x end"
                          val Abstr = fromString Concr
                                   in
                          print "Case 3 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog1.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 4 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog2.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 5 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog3.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 6 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog4.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 7 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "1::2::3::([Int] [])"
                          val Abstr = fromString Concr
                                   in
                          print "Case 8 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog5.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 9 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "hd (1 :: 2 :: ([Int] []))"
                          val Abstr = fromString Concr
                                   in
                          print "Case 10 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun next(Int y) = y + 1; next(6) = 7"
                          val Abstr = fromString Concr
                                   in
                          print "Case 11 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var f = fn (Int -> Int g) => g(3) end; f"
                          val Abstr = fromString Concr
                                   in
                          print "Case 12 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var f = fn (Int -> Int g) => g(3) end; f"
                          val Abstr = fromString Concr
                                   in
                          print "Case 13 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var e = ([Int] []); var x = 1::2::e; x"
                          val Abstr = fromString Concr
                                   in
                          print "Case 14 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var e = (1, !true);e"
                          val Abstr = fromString Concr
                                   in
                          print "Case 15 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun next(Int y) = y + 1; next(next(6))"
                          val Abstr = fromString Concr
                                   in
                          print "Case 16 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun add () = (5, 1-4); add()"
                          val Abstr = fromString Concr
                                   in
                          print "Case 17 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var y = (5, 1-4, (5,5)); y"
                          val Abstr = fromString Concr
                                   in
                          print "Case 18 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun add (Int x,Int y) = y + x; add(3,4)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 19 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun add (Int x) = fn (Int y) => y+x end; add(1)(3)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 20 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "ise(true::([Bool] []))"
                          val Abstr = fromString Concr
                                   in
                          print "Case 21 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var x = 4;var y = 3;print(x); x + y; print(y)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 22 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog7.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 23 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog8.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 24 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog9.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 25 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "PlcPrograms/Prog10.plc"
                          val Abstr = fromFile Concr
                                   in
                          print "Case 26 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "{fun f(Int x) = x + 1 ; fun g(Int x) = 2 * x ;f(2) + g(3)}"
                          val Abstr = fromString Concr
                                   in
                          print "Case 27 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var x = true; if x then 10 else 20"
                          val Abstr = fromString Concr
                                   in
                          print "Case 28 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var in = 5; fun f(Int x) = if in < 0 then -1 else if in = 0 then 0 else 1; f(in)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 29 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "{var x = 5+7; x}"
                          val Abstr = fromString Concr
                                   in
                          print "Case 30 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "var y = 11 ; fun f(Int x) = x + y ;var y = 22 ; f(y)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 31 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun inc(Int x) = x + 1; fun rec fib(Int n):Int = {fun ge2(Int x) = 1 < x ;  if ge2(n) then fib(n-1) + fib(n-2) else 1}; fib(25)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 32 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "fun f(Int x) = { fun g(Int y,Int z) = z * y ;g(x,x) + 1}; f(3)"
                          val Abstr = fromString Concr
                                   in
                          print "Case 33 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "(true,false)[1]"
                          val Abstr = fromString Concr
                                   in
                          print "Case 34 parsed\n";(Concr, Abstr)
                                   end) ::
                                   (let
                          val Concr = "((5,6),false)[1][2]"
                          val Abstr = fromString Concr
                                   in
                          print "Case 35 parsed\n";(Concr, Abstr)
                                   end) ::
                                   [];

val bad = 	(let
   val Concr = "PlcPrograms/Prog6.plc"
   val Abstr = fromFile Concr
            in
   print "Case 36 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "fun rec ignore (Int x):Nil = (); ignore(true)"
   val Abstr = fromString Concr
            in
   print "Case 37 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "if 2 = true then 1 else 0"
   val Abstr = fromString Concr
            in
   print "Case 38 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "if 2 != true then 1 else 0"
   val Abstr = fromString Concr
            in
   print "Case 39 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "fun rec bad (Int x):Bool = x+1; bad(4)"
   val Abstr = fromString Concr
            in
   print "Case 40 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "if 1 < 2 then 1 else false"
   val Abstr = fromString Concr
            in
   print "Case 41 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "if 10 then 1 else 0"
   val Abstr = fromString Concr
            in
   print "Case 42 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "match 10 with end"
   val Abstr = fromString Concr
            in
   print "Case 43 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "match 10 with | 0 -> 2 | _ -> true end"
   val Abstr = fromString Concr
            in
   print "Case 44 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "match 10 with | false -> false | _ -> true end"
   val Abstr = fromString Concr
            in
   print "Case 45 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "var x = 12; x(1)"
   val Abstr = fromString Concr
            in
   print "Case 46 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "(1, false)[3]"
   val Abstr = fromString Concr
            in
   print "Case 47 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "([Bool][])[1]"
   val Abstr = fromString Concr
            in
   print "Case 48 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "hd (([Int] []))"
   val Abstr = fromString Concr
            in
   print "Case 49 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "tl (([Int] []))"
   val Abstr = fromString Concr
            in
   print "Case 50 parsed\n";(Concr, Abstr)
            end) ::
            (let
   val Concr = "match 10 with | 11 -> false | 12 -> true end"
   val Abstr = fromString Concr
            in
   print "Case 51 parsed\n";(Concr, Abstr)
            end) ::
            [];

