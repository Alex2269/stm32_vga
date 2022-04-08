# VGA-stm32f401


```

essentially this is an example of using DMA
not the VGA output graphics library
no standard resolution 240x200
low probability of operation in this mode,
I testing an old monitor SyncMaster 152b


```


```
 signals  : PB0       = HSync-Signal  13
            PC13      = VSync-Signal  14
            PB8+PB9   = color Blue    3
            PB10-PB12 = color Green   2
            PB13-PB15 = color red     1

 details see main.h

```

