LET count
READ count
IF count < 0
    GOTO end
ENDIF
LET sum
sum = 0
WHILE count > 0
    LET x
    READ x
    sum = sum + x
    count = count - 1
DONE
PRINT sum
LABEL end	