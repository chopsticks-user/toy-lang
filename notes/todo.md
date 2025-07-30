- ~~"defer" and "preface" statements that can only be used inside a function scope~~
- ~~"yield" statement that can be used inside any arbitrary scope~~
- ~~Update operators in the "syntax" module~~
- Complete test cases for parsing statements
- Work on LocationTracker and ErrorCollector
- Revise nodes' implementations, remove unnecessary methods
- Implement anonymous identifier
- Add more test cases for lexing and parsing
- Design trait, class, etc. Complete implementations and test cases
  for global definitions
- Create a lexeme class having both string and integer values
  (basically an enum convertible to a string).
- Replace "Vec<T> const&" and "Str const&" with Span<const T> and "StrV"
  respectively
- Create a visitor to collapse single-element tuples into
  value expressions
- Create a visitor to combine "x |> y(...)" into "y(x, ...)" 