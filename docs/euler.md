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
  futurePrimes = rest |> filter (not . divisibleBy startingAt)) |> primes
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
  futurePrimes = rest |> filter (not . divisibleBy startingAt)) |> primes
  n : futurePrimes
}

solution = primes (2 ..< 2000000) |> sum
```
