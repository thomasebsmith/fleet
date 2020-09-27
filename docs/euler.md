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
divides = x -> y -> x % y == 0
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

## Problem 5
```fleet
scope (import "Integer")
solution = 1 ..<= 20 |> reduceLeft lcm
```

## Problem 6
```fleet
scope (import "Tuple")

firstSum = 1 ..<= 10 |> map (** 2) |> reduceLeft (+)
secondSum = 1 ..<= 10 |> reduceLeft (+) |> (** 2)

solution = secondSum - firstSum
```

## Problem 7
```fleet
scope (import "Sequence")
divisibleBy = x -> y -> y % x == 0
primes = [] -> []
primes = (n : rest) -> {
  futurePrimes = rest |> filter (not . divisibleBy n)) |> primes
  n : futurePrimes
}
solution = arithmetic 2 1 |> primes |> tryAt 10001
```

## Problem 8
```fleet
number = "\
73167176531330624919225119674426574742355349194934\
96983520312774506326239578318016984801869478851843\
85861560789112949495459501737958331952853208805511\
12540698747158523863050715693290963295227443043557\
66896648950445244523161731856403098711121722383113\
62229893423380308135336276614282806444486645238749\
30358907296290491560440772390713810515859307960866\
70172427121883998797908792274921901699720888093776\
65727333001053367881220235421809751254540594752243\
52584907711670556013604839586446706324415722155397\
53697817977846174064955149290862569321978468622482\
83972241375657056057490261407972968652414535100474\
82166370484403199890008895243450658541227588666881\
16427171479924442928230863465674813919123162824586\
17866458359124566529476545682848912883142607690042\
24219022671055626321111109370544217506941658960408\
07198403850962455444362981230987879927244284909188\
84580156166097919133875499200524063689912560717606\
05886116467109405077541002256983155200055935729725\
71636269561882670428252483600823257530420752963450"

largestProduct = nums :: Sequence Int & (length |. (>= 13)) -> bestSoFar -> {
  theProduct = product (nums |> take 13)
  bestSoFar = if (theProduct > bestSoFar) then theProduct else bestSoFar
  largestProduct (nums |> drop 1) bestSoFar
}
largestProduct = _ -> bestSoFar -> bestSoFar

digits = number |> tryMap (tryParseAs Int)
solution = largestProduct digits 0
```

## Problem 9
```fleet
isSolution = (a, b) -> (1000 - a - b) ** 2 == a ** 2 + b ** 2
makeSolution = (a, b) -> (a, b, 1000 - a - b)

solution = zip (1 ..<= 500) (1 ..<= 500) |> filter isSolution |> makeSolution
```

## Problem 10
```fleet
primes = [] -> []
primes = (n : rest) -> {
  futurePrimes = rest |> filter (not . divisibleBy n)) |> primes
  n : futurePrimes
}

solution = primes (2 ..< 2000000) |> sum
```

## Problem 11
```fleet
grid_string = "\
08 02 22 97 38 15 00 40 00 75 04 05 07 78 52 12 50 77 91 08
49 49 99 40 17 81 18 57 60 87 17 40 98 43 69 48 04 56 62 00
81 49 31 73 55 79 14 29 93 71 40 67 53 88 30 03 49 13 36 65
52 70 95 23 04 60 11 42 69 24 68 56 01 32 56 71 37 02 36 91
22 31 16 71 51 67 63 89 41 92 36 54 22 40 40 28 66 33 13 80
24 47 32 60 99 03 45 02 44 75 33 53 78 36 84 20 35 17 12 50
32 98 81 28 64 23 67 10 26 38 40 67 59 54 70 66 18 38 64 70
67 26 20 68 02 62 12 20 95 63 94 39 63 08 40 91 66 49 94 21
24 55 58 05 66 73 99 26 97 17 78 78 96 83 14 88 34 89 63 72
21 36 23 09 75 00 76 44 20 45 35 14 00 61 33 97 34 31 33 95
78 17 53 28 22 75 31 67 15 94 03 80 04 62 16 14 09 53 56 92
16 39 05 42 96 35 31 47 55 58 88 24 00 17 54 24 36 29 85 57
86 56 00 48 35 71 89 07 05 44 44 37 44 60 21 58 51 54 17 58
19 80 81 68 05 94 47 69 28 73 92 13 86 52 17 77 04 89 55 40
04 52 08 83 97 35 99 16 07 97 57 32 16 26 26 79 33 27 98 66
88 36 68 87 57 62 20 72 03 46 33 67 46 55 12 32 63 93 53 69
04 42 16 73 38 25 39 11 24 94 72 18 08 46 29 32 40 62 76 36
20 69 36 41 72 30 23 88 34 62 99 69 82 67 59 85 74 04 36 16
20 73 35 29 78 31 90 01 74 31 49 71 48 86 81 16 23 57 05 54
01 70 54 71 83 51 54 69 16 92 33 48 61 43 52 01 89 19 67 48"

grid = grid_string |> split '\n' |> map (split ' ') |>
  map (map (tryParseAs Int))

height = length grid


solution = if (height > 0) then {
  width = grid |> map length |> min
  getProduct = map ((row, col) -> grid @ row @ col) |. product
  diagonals = zip (0 ..< height) (0 ..< width) |> concatMap ((row, col) -> {
    diag1 = [
      (row, col), (row + 1, col), (row + 2, col), (row + 3, col)
    ]
    diag2 = [
      (row, col), (row + 1, col + 1), (row + 2, col + 2), (row + 3, col + 3)
    ]
    diag3 = [
      (row, col), (row, col + 1), (row, col + 2), (row, col + 3)
    ]
    if (row + 3 < height && col + 3 < width) then [diag1, diag2, diag3] else (
      if (row + 3 < height) then [diag1] else (
      if (col + 3 < width) then [diag3] else []))
  })
  diagonals |> map getProduct |> max
} else 0
```
