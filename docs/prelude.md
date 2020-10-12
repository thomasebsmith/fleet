# Prelude Documentation

## `(%) :: Num => Num (/= 0) => Num`
The modulo operator, `%`, returns the remainder after `dividend` is divided by
`divisor`.

### Usage
`remainder :: Num = (dividend :: Num) % (divisor :: Num (/= 0))`

### Examples
```fleet
divisibleByThree = x :: Num -> x % 3 == 0

divisibleByThree  0 == True
divisibleByThree  2 == False
divisibleByThree -1 == False
divisibleByThree 12 == True
```

## `(+) :: Num => Num => Num`
The addition operator, `+`, returns the sum of `first_number` and
`second_number`.

### Usage
`sum :: Num = (first_number :: Num) + (second_number :: Num)`

### Examples
```fleet
generate_fibonacci = x :: Int -> y :: Int -> (x + y) :
  generate_fibonacci y (x + y)

fibonacci_sequence = generate_fibonacci 0 1
```

## `(-) :: Num => Num => Num`
The subtraction operator, `-`, returns the difference of `first_number` and
`second_number`.

### Usage
`difference :: Num = (first_number :: Num) - (second_number :: Num)`

### Notes
This should not be confused with negative numeric literals (e.g. `-1.3`).
When a `-` immediately precedes a numeric literal, it is *not* interpreted
as subtraction.

## `(*) :: Num => Num => Num`
The multiplication operator, `*`, returns the product of `first_number` and
`second_number`.

### Usage
`product :: Num = (first_number :: Num) * (second_number :: Num)`

### Examples
```fleet
factorial = 0 -> 1
factorial = n -> n * factorial (n - 1)

factorial 5 == 120
factorial 1 == 1
factorial 3 == 6
```

## `(/) :: Num => Num (/= 0) => Num`
The multiplication operator, `/`, returns the quotient of `dividend` and
`divisor`.

### Usage
`quotient :: Num = (dividend :: Num) * (divisor :: Num)`

### Examples
```fleet
divisionSequence = divisor -> n -> n : (divisionSequence divisor (n / divisor))
halves = divisionSequence 2 1

halves |> take   10 |> sum |> (< 2)
halves |> take   20 |> sum |> (< 2)
halves |> take 2000 |> sum |> (< 2)
```
