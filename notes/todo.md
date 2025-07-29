- ~~"defer" and "preface" statements that can only be used inside a function scope~~
- ~~"yield" statement that can be used inside any arbitrary scope~~
- Create a lexeme class having both string and integer values
  (basically an enum convertible to a string).
- Update operators in the "syntax" module
- Create a visitor to collapse single-element tuples into
  value expressions
- Replace "Vec<T> const&" and "Str const&" with Span<const T> and "StrV"
  respectively
- Create a visitor to combine "x |> y(...)" into "y(x, ...)" 