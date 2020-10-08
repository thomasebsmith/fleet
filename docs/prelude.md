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
