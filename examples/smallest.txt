LET n
READ n
LET arr[n]
LET i
i = 0
WHILE i < n
    READ arr[i]
    i = i + 1
DONE
LET smallest
smallest = arr[0]
i = 1
WHILE i < n
    IF arr[i] < smallest
        smallest = arr[i]
    ENDIF
DONE
PRINT smallest