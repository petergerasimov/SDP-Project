LET secret
secret = 15
LET guess
guess = 0
WHILE guess != secret
    READ guess
    IF guess < secret
        PRINT 1355
    ENDIF
    IF guess > secret
        PRINT 816
    ENDIF
DONE
PRINT 1337