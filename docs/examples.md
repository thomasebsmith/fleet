# Fleet Examples

## Fizzbuzz
```fleet
fizzbuzz = n -> if (n % 3 == 0 && n % 5 == 0) then "FizzBuzz" else (
                if (n % 3 == 0) then "Fizz" else (
                if (n % 5 == 0) then "Buzz" else (n |> toString)))

main = 1 ..<= 100 |> map fizzbuzz |> join '\n' |> print
```

## Length of last word in string
```fleet
string = import "string"

lengthOfLastWord = splitWith string.isWhitespace |.
                   filter (not . empty) |.
                   lastOr "" |.
                   length
```
