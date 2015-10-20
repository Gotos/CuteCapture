# What is Cute 3DS Capture?

Cute 3DS Capture is a 3DS capturing software for linux (and possibly Mac OS X, although this is not yet tested). It is written in C++ and uses [SFML](http://www.sfml-dev.org/).

# How to compile

Install SFML and the SFML-headers (libsfml-dev or similar) as well as libusb-1.0.0 and the headers (libusb-1.0.0-dev). Then, simple do ```./autogen.sh && ./configure && make```.

# How to use

Start Cute3DSCapture. You can autoresize the window with 1-0 (1x, 1.5x, 2x,... original size). Press the spacebar to toggel window split.

If Cute3DSCapture doesn't detect your 3DS and you are using linux, copy 95-3dscapture.rules to /etc/udev/rules.d and reload udev-rules with ```udevadm control --reload-rules```.

# Why "Cute" 3DS Capture?

It was a play on words when I first tried to design a capture software with Qt (which is prononced "cute"). Although I didn't say with Qt, I liked the name.
