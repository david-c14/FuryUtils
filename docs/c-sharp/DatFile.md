# DatFile

a class to handle [datfiles](../formats/datfile.md)

## implements
    System.IDisposable

## constructors
    
[`DatFile()`](DatFile.DatFile.md) Construct an empty DatFile archive for writing.
[`DatFile(byte[])`](DatFile.DatFile.md) Construct a DatFile archive for reading.

## properties

[`int Count`](DatFile.Count.md) The number of content files in the DatFile archive.

## methods

[`void Add(string, byte[], bool)`](DatFile.Add.md) Add a content file to the archive, with our without compression.
[`byte[] Buffer()`](DatFile.Buffer.md) Get the entire archive content a byte buffer.
[`IEnumerable<DatFileEntry> Entries`](DatFile.Entries.md) The content files in the archive.
[`DatFileEntry Item(int)`](DatFile.Item.md) A single content file in the archive by ordinal position.

