# What is CuteCapture?

CuteCapture is a collection of a DS and 3DS capturing tool for linux and Mac OS X for use with the capture boards by [3dscapture.com](http://3dscapture.com). It is written in C++ and uses [SFML](http://www.sfml-dev.org/).

# How to compile

Install SFML and the SFML-headers (libsfml-dev or similar) as well as libusb-1.0.0 and the headers (libusb-1.0.0-dev). Then, simple do `./autogen.sh && ./configure && make`. If you only want Cute 3DS Capture or Cute DS Capture, replace `make` with `make Cute3DSCapture` or `make CuteDSCapture`.

# How to use

Start CuteDSCapture or Cute3DSCapture, depending on your console. You can autoresize the window with 1-0 (1x, 1.5x, 2x,... original size). Press the spacebar to toggle window split. In Cute 3DS Capture, press C to toggle cropping to the original DS resolution (you can start DS titles in the original resolution when playing on 3DS by hold START or SELECT when launching the game).

If CuteCapture doesn't detect your handheld console and you are using linux, copy `95-3dscapture.rules` and/or `95-dscapture.rules` to /etc/udev/rules.d and reload udev-rules with `udevadm control --reload-rules`. You can also just run `install_udev_rules.sh`, which does this for you.

# Why "Cute" Capture?

It was a play on words when I first tried to design a capture software for 3DS with Qt (which is prononced "cute"). Although I didn't stay with Qt, I liked the name Cute 3DS Capture and called the "port" for DS Cute DS Capture. Since I've merged their codebases into one, they are collectively called CuteCapture now.