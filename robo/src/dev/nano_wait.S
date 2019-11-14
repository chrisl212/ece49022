.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.global nano_wait
nano_wait:
    subs r0,#83
    bgt nano_wait
    bx lr
