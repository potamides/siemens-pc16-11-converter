# Siemens PC 16-11 Keyboard Converter
This project contains instructions and code on how to build a USB converter for
the [Siemens PC 16-11](https://www.tecmumas.de/de/computer/siemens_pc1611.htm)
keyboard. It makes use of the [tmk_core](https://github.com/tmk/tmk_core)
keyboard firmware library and was tested with the [Pro
Micro](https://joy-it.net/en/products/ARD_Pro-Micro) microcontroller, but other
Atmel AVR ATmega32U4 based boards should work as well.

## Wiring
All wiring information comes from the official manual which someone kindly
scanned and uploaded
[here](https://oldcomputers-ddns.org/public/pub/rechner/siemens/pc16-11/manuals/index.html).
I connected the `+RxD` pin to the `Rx` pin of the Pro Micro. Since we don't
need `-RxD` I connected it to `GND` with a 2.2 kOhm resistor. You might also
need to close the `J1` sodler bridge on the Pro Micro:

![wiring](https://github.com/user-attachments/assets/dca9f12e-27fa-489d-b6b8-9cd6551309f1)

## Build Instructions
Make sure you have [avr-gcc](https://gcc.gnu.org/wiki/avr-gcc),
[avr-libc](https://github.com/avrdudes/avr-libc), and
[avrdude](http://savannah.nongnu.org/projects/avrdude) installed. Then clone
the repository with the following command:
```sh
git clone --recurse-submodules https://github.com/potamides/siemens-pc16-11-converter
```
To build the firmware and program the controller change into the project
directory and run these commands:
```sh
make
avrdude -patmega32u4 -cavr109 -b57600 -Uflash:w:pc16_11_usb.hex -P /dev/ttyACM0
```
Prebuilt binaries can be downloaded
[here](https://github.com/potamides/siemens-pc16-11-converter/releases/latest/download/pc16_11_usb.hex).

You can change the keymap in `keymap.c`. You can find a list of possible key
symbols in
[tmk_core/doc/keycode.txt](https://github.com/tmk/tmk_core/blob/master/doc/keycode.txt).
Look at
[tmk_core/doc/keymap.md](https://github.com/tmk/tmk_core/blob/master/doc/keymap.md)
for more details.

## Additional Information
The three red keys in the upper right section of the keyboard do only send
keycodes when they are pressed together with `CTRL`. Pressing the right red key
together with `Shift` doesn't produce a keycode but toggles the internal beeper
speaker of the keyboard.

The keyboard contains an [EPROM](https://en.wikipedia.org/wiki/EPROM) which
holds the program that the keyboard runs. Since EPROMs are prone to data rot I
made a back up
[here](https://github.com/potamides/siemens-pc16-11-converter/releases/latest/download/eprom-dump.bin).
