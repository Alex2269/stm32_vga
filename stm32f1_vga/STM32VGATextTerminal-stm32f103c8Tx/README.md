STM32VGATextTerminal
====================

Проект текстового терминала на микроконтроллере stm32f103c8Tx. Проект предназначен 
для вывода текстовой информации на мониторы с интерфейсом VGA. Поддерживает 
разрешение 640х480 в монохромном режиме, размер шрифта 8x16, что позволяет 
уместить на экран 80x30 символов. 

Link: https://github.com/vasyaod/STM32VGATextTerminal

Author: vasyaod (vasyaod@mail.ru)

```c
simple change timer settings tim5 to tim3 for cpu stm32f103c8Tx (bluepill)
change pins:

VGA    descryption STM
1,2,3  VIDEO       PA7
13     HSYNC       PB1
14     VSYNC       PA0
```
