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
