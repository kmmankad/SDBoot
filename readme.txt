SDBoot for PSoC3
--------------

This is a proof-of-concept SD Card based bootloader for the PSoC3 family of SoCs from Cypress Semiconductors.

The Bootloader will write a pre-specified file from an attached SD Card(SPI interfaced) to
 the chip's flash,and execute a software reset following that.

This is actually a stock USB HID bootloader that has been given this extra functionality.After an SD card bootload
attempt,the bootloader will enumerate as a USB HID too,and can accept the program CYACD file from there as well.
Hence this extra feature can be added easily to any kind of existing bootloader,or can also function standalone.

This code has been tested with the CY8CKIT-030,and the microSD module from www.embeddedmarket.com with PSoC Creator updated to Component Pack 3.

-kmmankad

http://kmmankad.blogspot.com

This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License.