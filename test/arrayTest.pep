LET size
size = 10
LET a[size]
LET i
i = 0
WHILE i < size
    a[i] = i + 10
    i = i + 1
DONE
i = 0
WHILE i < size
    PRINT a[i]
    i = i + 1
DONE