; example.as

; this file will demonstrate a proper input file for this Assembly compiler.

; it contains examples for compilable Assembly lines, 
; such as the comment lines and empty line shown above.

; first, declaring code line labels and a simple mov command:
MAIN: mov #5, r0

; .data and .string labels (and without labels)
DATA: .data 5, 8, -19
STR: .string "a string"
.data 3, -15
.string "HELLO WORLD!"

; demonstrating more combinations of addressing methods:
mov r0, DATA
add #3, DATA

; let's sprinkle some spaces and tabs here, just for fun :)
   sub          DATA   ,    r0               
cmp                 DATA,STR

; using an extern, then declaring it:
lea EXT, DATA
.extern EXT

; declaring an extern, the using it:
.extern EXT2
mov EXT2, r7

; lets declare MAIN label as entry:
.entry MAIN

; declare an entry before using it:
.entry ENTR
clr r2
ENTR: jmp &MAIN

; jumping to a label before it is declared
bne EXIT
jsr &Incer

EXIT: stop
Incer: inc r0

; a few more commands
dec r1
not r1
red STR
rts
stop
