# Project Euler Solutions
This page contains an incomplete list of solutions to
[Project Euler](https://projecteuler.net) problems.

## Problem 1
```fleet
divisible = n -> n % 3 == 0 || n % 5 == 0
solution = 1 ..< 1000 |> filter divisible |> sum
```

## Problem 2
```fleet
fibonacci x y = (x + y) : fibonacci y (x + y)

even = n -> n % 2 == 0
solution = fibonacci 1 2 |> while (<= 4000000) |> filter even |> sum
```

## Problem 3
```fleet
divides = x -> y -> y % x == 0
smallestPrimeFactor = n -> 2 ..<= n |> filter (divides n) |> first

solve = n -> {
  factor = smallestPrimeFactor n
  if (factor == n) then factor else (solve (n / factor))
}

solution = solve 600851475143
```

## Problem 4
```fleet
scope (import "Sequence")

isPalindrome = [] -> True
isPalindrome = [el] -> True
isPalindrome = seq -> {
  (last, seq) = splice seq (lastIndex seq)
  first : seq = seq
  first == last && isPalindrome seq
}

threeDigitProducts = (*) <$> (0 ..< 1000) <*> (0 ..< 1000)

solution = threeDigitProducts |> filter (toString |. isPalindrome) |> tryLast
```
