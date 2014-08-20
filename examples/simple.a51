        using   0               ; Register bank 0 will be in use

dseg
        org     030h            ; Start filling the data segment from here

cseg
        org     0               ; Reset
        jmp     _Int0
        org     3               ; External interrupt 0
        jmp     _Int0

        org     030h            ; Start filling the code segment from here
l1:
        nop
        jmp     l1

_Int0:
        ret