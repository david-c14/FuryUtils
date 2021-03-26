# DatFile

DatFile is a command line utility for packing, and unpacking [DatFile](../formats/datfile.md) archives

## Usage

DatFile *option* [*archivefile* [*contentfile*] [...]]]

### Options

**-?** 

    DatFile -?

show usage message.

---

**-l** 

    DatFile -l ARCHIVE.DAT

list all the *contentfiles* in the *archivefile*.

---

**-x**

    DatFile -x ARCHIVE.DAT ENTRY.TXT
    DatFile -x ARCHIVE.DAT ENTRY.TXT MUSIC.KMD
    
extract the specified *contentfile(s)* from the *archivefile*.

---

**-X**

    DatFile -X ARCHIVE.DAT
  
extract all the files in the *archivefile*.

---

**-c**

    DatFile -c ARCHIVE.DAT ENTRY.TXT
    DatFile -c ARCHIVE.DAT ENTRY.TXT MUSIC.KMD
 
Create *archivefile*, compress the *contentfile(s)* and place them in the *archivefile*.

**-u**

    DatFile -u ARCHIVE.DAT ENTRY.TXT
    DatFile -u ARCHIVE.DAT ENTRY.TXT MUSIC.KMD
    
Create *archivefile* and place the *contentfile(s)* into the *archivefile* **without** compression.
