
```

/******** debug *****************
address:
SysTick_BASE  0xe000e010
set {int}0xe000e010=7

TIM1->ARR     0x4001002c
TIM2->ARR     0x4000002c
TIM2->CCR3    0x4000003c
TIM2->CCR4    0x40000040

print (gdb):
x/1d 0x4001002c
x/1d 0x4000002c
x/1d 0x4000003c
x/1d 0x40000040

set (gdb):
set {int}0x4001002c=12
set {int}0x4000002c=2687
set {int}0x4000003c=500
set {int}0x40000040=320

********************************/

```
