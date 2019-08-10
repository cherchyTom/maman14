; file prog3.as

;===non define entry instuction ===
.entry  NONE
;== entry label is a macro instuction ===
.entry  MACRO

    .entry  LIST
    .extern  W
    .define sz = 2
MAIN:         mov   r3, LIST[sz]
LOOP:         jmp   W
prn   #-5
;== use undefined label ===
mov   STR[5], NONE[2]
;== use macro as label ===
mov   STR[5], MACRO[2]
sub   r1, r4
cmp   K, #sz
bne   W
L1:           inc   L3
    .entry LOOP
bne   LOOP
END:          stop
    .define len = 4
STR:          .string  "abcdef"
LIST: .data   6, -9, len
K:	      .data  22
    .extern  L3
.define MACRO = 4








