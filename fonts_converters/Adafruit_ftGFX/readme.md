
```md
TrueType to Adafruit_GFX font converter.  Derived from Peter Jakobs'
Adafruit_ftGFX fork & makefont tool, and Paul Kourany's Adafruit_mfGFX.

NOT AN ARDUINO SKETCH.  This is a command-line tool for preprocessing
fonts to be used with the Adafruit_GFX Arduino library.

For UNIX-like systems.  Outputs to stdout; redirect to header file, e.g.:
  ./fontconvert ~/Library/Fonts/FreeSans.ttf 18 > FreeSans18pt7b.h

REQUIRES FREETYPE LIBRARY.  www.freetype.org

Currently this only extracts the printable 7-bit ASCII chars of a font.
Will eventually extend with som


```

```
example create fonts:


  ./fontconvert AvrileSans-Regular.ttf 2  >  AvrileSans_Regular2pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 4  >  AvrileSans_Regular4pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 6  >  AvrileSans_Regular6pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 8  >  AvrileSans_Regular8pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 10 >  AvrileSans_Regular10pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 12 >  AvrileSans_Regular12pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 14 >  AvrileSans_Regular14pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 16 >  AvrileSans_Regular16pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 20 >  AvrileSans_Regular20pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 24 >  AvrileSans_Regular24pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 28 >  AvrileSans_Regular28pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 32 >  AvrileSans_Regular32pt8b.h
  ./fontconvert AvrileSans-Regular.ttf 36 >  AvrileSans_Regular36pt8b.h

  ./fontconvert AvrileSerif-Regular.ttf 2  >  AvrileSerif_Regular2pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 4  >  AvrileSerif_Regular4pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 6  >  AvrileSerif_Regular6pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 8  >  AvrileSerif_Regular8pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 10 >  AvrileSerif_Regular10pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 12 >  AvrileSerif_Regular12pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 14 >  AvrileSerif_Regular14pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 16 >  AvrileSerif_Regular16pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 20 >  AvrileSerif_Regular20pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 24 >  AvrileSerif_Regular24pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 28 >  AvrileSerif_Regular28pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 32 >  AvrileSerif_Regular32pt8b.h
  ./fontconvert AvrileSerif-Regular.ttf 36 >  AvrileSerif_Regular36pt8b.h

```
