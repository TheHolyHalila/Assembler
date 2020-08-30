;Illegal comment
clr r0    ;A file full of errors
    ;this is also illegal
;move is not defined
move r3, r4

; Non existing label(r9)
cmp r0, r9

;Too many args
stop 3
mov r0, r1, r5

;too few operands:
clr
add r5

; Illegal (non alphaic first character) label
5abel: .data 5

;Too many arguments
label1: .data 1
label2: .data 2
bne label1, label2

;Useless label decleration
UselessLabel: .entry W

;too long line
                                                                                  
;Too few arguments
sub #3

;next line will be waaay too long:
someLABEL: .data 5,        3,         990,          3467            ,               25082       , 0

;a legal line, to show that no output is made even if a single line can be compiled:
clr r0

;reserved name:
mov: mov r0, r1

;misused symbols:
clr &label
jmp #label2

;misused addressing methods:
jmp r0
mov r0, #45

;non integer number
mov #45.3, r4

;invalid . expression:
.command SOMETHING


;missing info for . expression:
.entry
.extern
.data
.string

; comma errors:
.data ,4,5
.data 4 ,, 5
.data 4 5
.data 4, 5,

; .data and .string errors:
.data "haha im not a number"
.string 123
.string no quotation mark?!?!?!?!?!
.data 1283473275024620967240674096107

.string "notice the junk text at end of line" junk

; .extern errors
.extern EXTERN
.entry EXTERN
EXTERN: .data 3

EXTERN: clr r0

jmp EXTERN
jmp &EXTERN

;useless lable warning:
USELESS: .entry ENTRYLABEL

;duplicate decleration warnings:
.extern DUPLICATEEXTERN
.extern DUPLICATEEXTERN
.entry DUPLICATEENTRY
.entry DUPLICATEENTRY




;empty label:
EMPTYLABEL:



