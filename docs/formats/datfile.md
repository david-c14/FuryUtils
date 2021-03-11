# `.DAT`

## Overview

There are two `.DAT` archives in the windows distribution: `INTRO.DAT` and `LANG.DAT`. They are used by INTRO.EXE and LANG.EXE respectively.

Each is an archive of (optionally) compressed content used by those programs. In particular images, sounds and music files are required by the intro.

## Format

Each archive begins with a 2 byte integer: the number of files in the archive. This is followed by a 22 byte header, the compressed first file, 
the next 22 byte header, the second compressed file, and so on until the correct number of files have been included. 
Here the archive ends. 
There is no additional padding, even for odd-length files.

Each header consists of:
- 13 bytes containing a null-terminated ASCII name for the file. Typically this is in the DOS 8.3 format so 13 bytes is sufficient, including the terminator.
- 4 bytes containing the uncompressed length of the file (in bytes).
- 4 bytes containing the compressed length of the original file (in bytes).
- 1 byte containing either 0 if the file is compressed or 1 if the file is not compressed (typically because compression would yield a larger file).

If a file in the archive is not compressed, the header will contain the same value for 'Compressed' and 'Uncompressed' length.

It is possible to seek through the archive to a particular file by locating and reading the first header (at address 0x0002), then seeking forward to the next header by the length of the header, and the 
compressed length of the file. No word padding adjustments are required.

| DatFile Archive | Size |
| :---: | :---: |
| Number of Files | 2 |
| First Header | 22 |
| First Compressed File | Varying |
| Second Header | 22 |
| Second Compressed File | Varying | 
| ... | |

| Header | Size |
| :---: | :---: |
| Filename | 13 |
| Uncompressed Length | 4 |
| Compressed Length | 4 |
| Compressed Flag | 1 |
