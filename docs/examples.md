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

## Palindrome check
```fleet
scope (import "Sequence")

isPalindrome = [] -> True
isPalindrome = [el] -> True
isPalindrome = seq -> {
  (last, seq) = splice seq (lastIndex seq)
  first : seq = seq
  first == last && isPalindrome seq
}
```

## Numbers of lines in a file
```fleet
files = import "files"

filePath = files.homePath / "read-me.txt"

main = files.lines filePath |> map length
```

## Syntactic sugar for functions
```fleet
# This syntax:
function_1 = x -> x + 1

# Is equivalent to this sugar:
function_1 x = x + 1
```

## Multi-line comments
```fleet
#{
  This syntax indicates a multi-line comment.
  Multi-line comments can be nested: #{ Also multi-line. }
}
```
