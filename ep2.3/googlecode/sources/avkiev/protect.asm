$ARM9E
$genonly

VrezkaAB2   equ	092E044h
VrezkaGr2   equ 092E280h

VrezkaNew2  equ 092EB08h
VrezkaIn2   equ 092EC74h
VrezkaOut2  equ 092EDACh
VrezkaNewSMS2  equ 092EEE4h

VrezkaAB    equ	0931798h
VrezkaGr    equ 09317A4h

VrezkaNew   equ 0B23718h
VrezkaIn    equ 0B23738h
VrezkaOut   equ 0B23768h

Patch	 equ	16BB400h

$INCLUDE (..\S65_50.inc)

; ---------------------------------------------------------------------------
org VrezkaAB2
            dd      q1+EntryAB2
org VrezkaGr2
            dd      q1+EntryGr2

org VrezkaNew2
            dd      q1+EntryNew2
org VrezkaIn2
            dd      q1+EntryIn2
org VrezkaOut2
            dd      q1+EntryOut2
org VrezkaNewSMS2
            dd      q1+EntryNewSMS2

org VrezkaAB
            dd      q1+EntryAB
org VrezkaGr
            dd      q1+EntryGr

org VrezkaNew
            dd      q1+EntryNew
org VrezkaIn
            dd      q1+EntryIn
org VrezkaOut
            dd      q1+EntryOut

; ---------------------------------------------------------------------------
org Patch
; ---------------------------------------------------------------------------
            db      "Password Protect"
            db      "©aavkiev 13.06.05"
RunSec:     push    {r3,LR}

            CallARM GetProfileARM
            pop     {r3}
            add     r0, #1
            cmp     r0, #7
            bne     DoPatch

            blx     r3
            pop     {PC}

DoPatch:    LoadReg 0, aRama
            mov     r2, #0
            str     r2, [r0]
            mov     r1, #6         ; Type
            str     r1, [r0, #4h]
            LoadReg 1, d3
            str     r1, [r0, #8h]
            mov     r1, #0F8h
            str     r1, [r0, #0Ch]
            str     r2, [r0, #10h]
            LoadReg 2, d6
            str     r2, [r0, #14h]
            str     r3, [r0, #18h]
            bl      0x16AD6A2
            pop     {PC}
; ---------------------------------------------------------------------------
            Align4
d3:         dd    0xA1695447
d6:         dd    0xA8783AF4
aRama:      dd    Rama+40h
; ---------------------------------------------------------------------------
; ---------------------------------------------------------------------------
; ---------------------------------------------------------------------------
EntryAB:    
            dw      0x4B00
            b       RunSec
            dd      0xA08DF2C1
; ---------------------------------------------------------------------------
EntryAB2:   dw      0x4B00
            b       RunSec
            dd      0xA08D95C9
; ---------------------------------------------------------------------------
EntryGr:    dw      0x4B00
            b       RunSec
            dd      0xA08DF2F1
; ---------------------------------------------------------------------------
EntryGr2:   dw      0x4B00
            b       RunSec
            dd      0xA08DA0DD
; ---------------------------------------------------------------------------
EntryNew:   dw      0x4B00
            b       RunSec
            dd      0xA0A82B4D
; ---------------------------------------------------------------------------
EntryNew2:  dw      0x4B00
            b       RunSec
            dd      0xA08D9D83
; ---------------------------------------------------------------------------
EntryIn:    dw      0x4B00
            b       RunSec
            dd      0xA0A831C9
; ---------------------------------------------------------------------------
EntryIn2:   dw      0x4B00
            b       RunSec
            dd      0xA08D9DF5
; ---------------------------------------------------------------------------
EntryOut:   dw      0x4B00
            b       RunSec
            dd      0xA0A8330D
; ---------------------------------------------------------------------------
EntryOut2:  dw      0x4B00
            b       RunSec
            dd      0xA08DA075
; ---------------------------------------------------------------------------
EntryNewSMS2:   
            dw      0x4B00
            b       RunSec
            dd      0xA08D96B5
; ---------------------------------------------------------------------------
END
