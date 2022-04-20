    section .text
    global fp_mul_ext

; multiply two 32-bit fixed point integers (with scale 4096) using 64-bit math
; to avoid overflows using standard multiplication
;   a0: multiplicant
;   a1: multiplier
;   v0: result of multiplication
fp_mul_ext:
    mult a1, a0
    mflo v0
    mfhi t0
    srl  v0, 12
    sll  t0, 20
    j    ra
    or   v0, t0