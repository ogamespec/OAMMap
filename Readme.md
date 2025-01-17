# OAMMap

A small utility for creating a visual map of the PPU OAM.

![oammap_demo](/oammap_demo.png)

The map is presented as a set of squares, each square corresponds to one physical location of a memory cell on the chip.

The cells are color coded:
- LimeGreen: Byte 0 (Y)
- Ivory: Byte 1 (Index)
- Red: Byte 2 (Attr)
- Blue: Byte 3 (X)
- DarkViolet: Bytes for Temp OAM ( common color)

Inside each cell is a caption, of the form `AA-B`:
- AA: Cell address 00-FF
- B: The bit number of the corresponding byte.

Note that the OAM physically lacks cells for bits 2-4 attribute bytes (Byte 2). However, Temp OAM has a place for them, but zeros are always copied there.

## NTSC vs PPU

One of the distinguishing features in the NTSC version of the PPU is that the OAM address is fed in inverted form (`/OAM0-7`). This causes the logical column numbers to no longer correspond to their physical location.

The PPU developers also took this feature into account to connect rows for bits 2-4:

![pal_oam_row_outputs2](/pal_oam_col_outputs2.png)

(NTSC PPU to the left)

## How to view the map

The utility creates an .xml file, which should be opened in the [Deroute](https://github.com/emu-russia/Deroute) utility (File -> Add entities...)

## Pre-made OAM Maps

:warning: The pictures are big.

|NTSC OAM Map|PAL OAM Map|
|---|---|
|![oammap_ntsc](/oammap_ntsc.png)|![oammap_pal](/oammap_pal.png)|
