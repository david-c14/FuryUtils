﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace carbon14.FuryUtils
{
    public class DatFile : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
        internal struct DatFileHeader
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

        public class DatFileItem
        {
            private DatFileHeader _header;
            private readonly DatFile _datFile;
            private readonly UInt32 _index;

            internal DatFileItem(DatFileHeader header, DatFile datFile, UInt32 index)
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
                _datFile.CheckDisposed();
                byte[] buffer = new byte[UncompressedSize];
                if (DatFile_entry(_datFile.Pointer, _index, buffer, buffer.Length))
                {
                    return buffer;
                }
                FuryException.Throw();
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
            FuryException.Throw();
        }

        public DatFile(byte[] buffer)
        {
            _datFile = DatFile_create(buffer, buffer.Length);
            FuryException.Throw();
        }

        protected void CheckDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(DatFile));
        }

        protected IntPtr Pointer => _datFile;

        public int Count {
            get
            {
                CheckDisposed();
                return DatFile_entryCount(_datFile);
            }
        }

        public IEnumerable<DatFileItem> Items()
        {
            CheckDisposed();
            Reset();
            _index = 1;
            DatFileItem dfe = Next();
            while (dfe != null)
            {
                FuryException.Throw();
                yield return dfe;
                CheckDisposed();
                _index++;
                dfe = Next();
            }
        }

        protected void Reset()
        {
            DatFile_reset(_datFile);
            FuryException.Throw();
        }

        protected DatFileItem Next()
        {
            DatFileHeader header = new DatFileHeader();
            if (DatFile_next(_datFile, ref header))
            {
                return new DatFileItem(header, this, _index);
            }
            FuryException.Throw();
            return null;
        }

        public DatFileItem Item(int index)
        {
            CheckDisposed();
            DatFileHeader header = new DatFileHeader();
            if (DatFile_header(_datFile, (UInt32)index, ref header))
            {
                return new DatFileItem(header, this, (UInt32)index);
            }
            FuryException.Throw();
            return null;
        }

        public void Add(string fileName, byte[] buffer, bool compress)
        {
            CheckDisposed();
            byte[] fileNameBuffer = Encoding.ASCII.GetBytes(fileName);
            DatFile_add(_datFile, fileNameBuffer, buffer, buffer.Length, compress);
            FuryException.Throw();
        }

        public byte[] Buffer()
        {
            CheckDisposed();
            byte[] buffer = new byte[DatFile_size(_datFile)];
            if (DatFile_buffer(_datFile, buffer, buffer.Length))
            {
                return buffer;
            }
            FuryException.Throw();
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
