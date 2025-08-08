### Lexing and Parsing

- ~~"defer" and "preface" statements that can only be used inside a function scope~~
- ~~"yield" statement that can be used inside any arbitrary scope~~
- ~~Update operators in the "syntax" module~~
- ~~Complete test cases for parsing statements~~
- ~~Implement anonymous identifier~~
- ~~Create a lexeme class having both string and integer values~~
- ~~Work on LocationTracker and ErrorCollector~~
- ~~Replace "Vec<T> const&" and "Str const&" with Span<const T> and "StrV"
  respectively~~
- ~~Revise nodes' implementations, remove unnecessary methods~~
- ~~Function types must be right-associate~~
- ~~Vec<Node> implies the need for a wrapper~~
- ~~Implement parsing logic for strings.~~
- ~~Handle "try" expressions~~.
- Remove the "str" field and getter in token, use the one provided by lexeme instead
- Remove "invalid" lexeme, Lex should insert an "empty" lexeme at the end of the
  token buffer
- ~~Type params should be handled by a dedicated parsing method~~
- Replace Expected in parse with Opt
- Add test cases to ensure correct operator precedence
- Add more test cases for lexing and parsing to test ErrorCollector
- Design trait, class, etc. Complete implementations and test cases
  for global definitions

### Static phase

- Create a visitor to collapse single-element tuples into
  value expressions
- Create a visitor to combine "x |> y(...)" into "y(x, ...)"
- Create a general error collector for IO, memory, etc