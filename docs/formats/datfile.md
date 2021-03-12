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

If a file in the archive is not compressed, the header will contain the same value for 'Compressed' and 'Uncompressed' length, and the raw uncompressed file will be embedded in the archive.

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

If the file is compressed, it is composed of a series of compression frames.

Each compression frame starts with a single byte to be interpreted as up to 8 bit-flags, starting with the least significant bit.

If the flag is 1, then the next byte in the decompressed file is simply the next byte in the archive.

If the flag is 0, then the next two bytes in the archive are read and decoded to give an address and length of a sequence of bytes in the already decompressed data, 
that can be copied. So the compressed archive is a mix of raw bytes and references to previously encountered sequences of bytes.

The length of the sequence to be copied is the least significant 4 bits of the second byte, plus 3. (It's not worth copying sequences of less than 3 bytes, so these 4 bits represent a length of between 3 and 18 bytes)

The address of the first byte of the sequence is formed from the remaining 12 bits

| <sub>msb</sub> First Byte <sub>lsb</sub> | <sub>msb</sub> Second Byte <sub>lsb</sub> |
| :---: | :---: |
| bits 7-0 of the address | bits 11-8 of the address / bits 3-0 of the length |

The value of 18 should be added to the address.

However since the address is composed of only 12-bits. It represents an address within the last 4096 bytes already decompressed. It never refers to any point earlier than that, nor does it refer to a point after the most recently decompressed byte. The address should be adjustedy by adding or subtracting 4096 until it falls within this range.

After this process, an address of 0 will refer to the first decompressed byte (or the 4096th, or the 8192th etc), and an address of 1 will refer to the second decompressed byte.

if the address is negative, a space character 0x20 should be used.  Copy the number of bytes specified by the length, from the address to form newly decompressed bytes.

It is possible for the source and destination sequences to overlap, so the sequence should be copied from the lowest address upwards.
