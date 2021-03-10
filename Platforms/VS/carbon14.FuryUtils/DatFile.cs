using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace carbon14.FuryUtils
{
    public class DatFile : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
        public struct DatFileHeader
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 13)]
            public char[] FileName;
            [MarshalAs(UnmanagedType.U4)]
            public UInt32 UncompressedSize;
            [MarshalAs(UnmanagedType.U4)]
            public UInt32 CompressedSize;
            [MarshalAs(UnmanagedType.U1)]
            public bool IsNotCompressed;
        }

        public class DatFileEntry
        {
            private DatFileHeader _header;
            private readonly DatFile _datFile;
            private readonly UInt32 _index;

            public DatFileEntry(DatFileHeader header, DatFile datFile, UInt32 index)
            {
                _header = header;
                _datFile = datFile;
                _index = index;
            }

            public string FileName => new string(_header.FileName, 0, Array.IndexOf(_header.FileName, '\0'));
            public UInt32 UncompressedSize => _header.UncompressedSize;
            public UInt32 ComressedSize => _header.CompressedSize;
            public bool IsNotCompressed => _header.IsNotCompressed;
            public UInt32 Index => _index;

            public byte[] Buffer()
            {
                byte[] buffer = new byte[UncompressedSize];
                if (DatFile_entry(_datFile.Pointer, _index, buffer, buffer.Length))
                {
                    return buffer;
                }
                return null;
            }
        }

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr DatFile_createNew();

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr DatFile_create([MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DatFile_destroy(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int DatFile_entryCount(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DatFile_reset(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool DatFile_next(IntPtr datFile, ref DatFileHeader header);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool DatFile_header(IntPtr datFile, UInt32 index, ref DatFileHeader header);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool DatFile_entry(IntPtr datFile, UInt32 index, [MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DatFile_add(IntPtr datFile, [MarshalAs(UnmanagedType.LPArray)] byte[] fileName, [MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size, bool compress);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int DatFile_size(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool DatFile_buffer(IntPtr datFile, [MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size);

        readonly private IntPtr _datFile;
        private bool _disposed = false;
        private UInt32 _index = 0;

        public DatFile()
        {
            _datFile = DatFile_createNew();
        }

        public DatFile(byte[] buffer)
        {
            _datFile = DatFile_create(buffer, buffer.Length);
        }

        protected IntPtr Pointer => _datFile;

        public int EntryCount {
            get
            {
                return DatFile_entryCount(_datFile);
            }
        }

        public IEnumerable<DatFileEntry> Entries()
        {
            Reset();
            _index = 1;
            DatFileEntry dfe = Next();
            while (dfe != null)
            {
                yield return dfe;
                _index++;
                dfe = Next();
            }
        }

        protected void Reset()
        {
            DatFile_reset(_datFile);
        }

        protected DatFileEntry Next()
        {
            DatFileHeader header = new DatFileHeader();
            if (DatFile_next(_datFile, ref header))
            {
                return new DatFileEntry(header, this, _index);
            }
            return null;
        }

        public DatFileEntry Header(int index)
        {
            DatFileHeader header = new DatFileHeader();
            if (DatFile_header(_datFile, (UInt32)index, ref header))
            {
                return new DatFileEntry(header, this, (UInt32)index);
            }
            return null;
        }

        public void Add(string fileName, byte[] buffer, bool compress)
        {
            byte[] fileNameBuffer = Encoding.ASCII.GetBytes(fileName);
            DatFile_add(_datFile, fileNameBuffer, buffer, buffer.Length, compress);
        }

        public byte[] Buffer()
        {
            byte[] buffer = new byte[DatFile_size(_datFile)];
            if (DatFile_buffer(_datFile, buffer, buffer.Length))
            {
                return buffer;
            }
            return null;
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (_disposed)
            {
                return;
            }

            if (disposing)
            {
                // free any managed objects here
            }

            DatFile_destroy(_datFile);

            _disposed = true;
        }


    }
}
