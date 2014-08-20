        extrn  _Int0
        public cnt1

        using   0               ; Register bank 0 will be in use

dseg
        org     030h            ; Start filling the data segment from here
cnt1:   ds      1

cseg
        org     0               ; Reset
        jmp     _Int0
        org     3               ; External interrupt 0
        jmp     _Int0

l1:
        nop
        jmp     l1
