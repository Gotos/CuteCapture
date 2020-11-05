# What is CuteCapture?

CuteCapture is a collection of a DS and 3DS capturing tool for linux and Mac OS X for use with the capture boards by [3dscapture.com](http://3dscapture.com). It is written in C++ and uses [SFML](http://www.sfml-dev.org/).

# How to install

Right now, CuteCapture will only be installed where you unpack the zip you will download in the steps below. It will not be installed system-wide.

## MacOS

Go to [the releases page](https://github.com/Gotos/Cute3DSCapture/releases) and download `CuteCapture.mac.zip`. Inside the zip, you will find two app bundles, one for Cute3DSCapture, one for CuteDSCapture. Just run the one appropriate for your handheld.

## Linux

To run CuteCapture, you will need to first install SFML and libusb-1.0. On a Debian-based distribution like Ubuntu you are likely able to install these by executing `sudo apt-get install libusb-1.0-0 libsfml`.

During installation, try to pay attention to the SFML version that is being installed. On modern distributions, at the time of writting, the version will likely be 2.4 or 2.5. If you notice which version was installed, you can try the rest for both version.

Go to [the releases page](https://github.com/Gotos/Cute3DSCapture/releases) and download either `CuteCapture-sfml-2.4.linux.zip` or `CuteCapture-sfml-2.5.linux.zip`, depending on your installed SFML version. Inside the zip, you will find two executables, `CuteDSCapture` and `Cute3DSCapture`. Just run the one appropriate for your handheld.

If CuteCapture only shows a blank window instead of connecting to your handheld, just run `install_udev_rules.sh` in a terminal. This will install udev rules to allow CuteCapture to connect to your handheld via USB.

If CuteCapture doesn't open a window whatsoever, you likely either didn't install libusb or SFML correctly or are trying to use the build for the wrong SFML version. Try downloading the other zip and see if the files in that one work.

# How to compile

Install SFML and the SFML-headers (libsfml-dev or similar) as well as libusb-1.0.0 and the headers (libusb-1.0.0-dev). Then, simple do `./autogen.sh && ./configure && make`. If you only want Cute 3DS Capture or Cute DS Capture, replace `make` with `make Cute3DSCapture` or `make CuteDSCapture`.

# How to use

Start CuteDSCapture or Cute3DSCapture, depending on your console.

Resizing the window: `1`-`9` or `0` will resize the resolution to 1x, 1.5x, 2x,... the original resolution of the handheld.

Window Split: Press `space` to toggle between one window containing both screens of the handheld and two individual windows.

DS Crop: In Cute3DSCapture, press `C` to toggle cropping to the original DS resolution (you can start DS titles in the original resolution when playing on 3DS by hold START or SELECT when launching the game).

# Why "Cute" Capture?

It was a play on words when I first tried to design a capture software for 3DS with Qt (which is pronounced "cute"). Although I didn't stay with Qt, I liked the name Cute 3DS Capture and called the "port" for DS Cute DS Capture. Since I've merged their codebases into one, they are collectively called CuteCapture now.