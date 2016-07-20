# MaQuis

**My humble Qt Image Archiving Tool**

Copyright 2016 by Alexander Koch


## About

MaQuis was born to bundle my workflow for digitalized (i.e. scanned)
photographs in one utility:
* rotate (losslessly), as the scanner does not set an EXIF rotation tag
* set an EXIF timestamp
* rename according to timestamp

It uses *libjpeg-turbo* for JPEG rotation and *exiv2* for EXIF manipulation.

TODO: Insert screenshot here


## Building

Make sure you have the following requirements installed:
* Qt (tested with qt-5.7)
* libjpeg-turbo (tested with 1.5.0)
* exiv2 (tested with 0.25)

(Note: exiv2-code is currently under development and not included, yet)

The code is supplied as *qmake* project so either use QtCreator or compile
manually:
    $ qmake MaQuis.pro -r -spec linux-g++
    $ make -j4

There currently is no installation mechanism, the binary is located in *bin/*.

## Usage

Just run the binary with files or folders as arguments:
    $ ./MaQuis /path/to/some/images /images/img1.jpeg /images/img2.jpg

Functionality of the main (and only) window should be self-explanatory. You may
use the following vim-style bindings:
* `j`/`k`: next/previous file
* `h`/`l`: rotate 90 degrees ccw/cw (not implemented yet)
* `r`: reload current file
* `f`: flip vertically (not implemented yet)

## Contributing

I provide this _as-is_, in hope that it might be an inspiration or starting
vector for someone. I don't plan expanding it to become a real image viewer or
feature-rich image manipulation tool.

However, pull requests for improvements on this basic functionality are always
welcome.

## License
This work is published under the terms of the MIT License, see file `LICENSE`.

