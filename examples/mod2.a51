        extrn   cnt1
        public  _Int0

        using   0               ; Register bank 0 will be in use

dseg
cnt2:   ds      1

cseg

_Int0:
        inc     cnt2
        inc     cnt1
        ret