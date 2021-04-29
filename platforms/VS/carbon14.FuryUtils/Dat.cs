using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace carbon14.FuryUtils
{
    public class Dat : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Ansi)]
        internal struct DatHeader
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

        public class DatItem
        {
            private DatHeader _header;
            private readonly Dat _dat;
            private readonly UInt32 _index;

            internal DatItem(DatHeader header, Dat dat, UInt32 index)
            {
                _header = header;
                _dat = dat;
                _index = index;
            }

            public string FileName => new string(_header.FileName, 0, Array.IndexOf(_header.FileName, '\0'));
            public UInt32 UncompressedSize => _header.UncompressedSize;
            public UInt32 ComressedSize => _header.CompressedSize;
            public bool IsNotCompressed => _header.IsNotCompressed;
            public UInt32 Index => _index;

            public byte[] Buffer()
            {
                _dat.CheckDisposed();
                byte[] buffer = new byte[UncompressedSize];
                if (Dat_entry(_dat.Pointer, _index, buffer, buffer.Length))
                {
                    return buffer;
                }
                FuryException.Throw();
                return null;
            }
        }

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Dat_createNew();

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Dat_create([MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Dat_destroy(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int Dat_entryCount(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Dat_reset(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Dat_next(IntPtr datFile, ref DatHeader header);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Dat_header(IntPtr datFile, UInt32 index, ref DatHeader header);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Dat_entry(IntPtr datFile, UInt32 index, [MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Dat_add(IntPtr datFile, [MarshalAs(UnmanagedType.LPArray)] byte[] fileName, [MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size, bool compress);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int Dat_size(IntPtr datFile);

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Dat_buffer(IntPtr datFile, [MarshalAs(UnmanagedType.LPArray)] byte[] buffer, int size);

        readonly private IntPtr _dat;
        private bool _disposed = false;
        private UInt32 _index = 0;

        public Dat()
        {
            _dat = Dat_createNew();
            FuryException.Throw();
        }

        public Dat(byte[] buffer)
        {
            _dat = Dat_create(buffer, buffer.Length);
            FuryException.Throw();
        }

        protected void CheckDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(Dat));
        }

        protected IntPtr Pointer => _dat;

        public int Count {
            get
            {
                CheckDisposed();
                return Dat_entryCount(_dat);
            }
        }

        public IEnumerable<DatItem> Items()
        {
            CheckDisposed();
            Reset();
            _index = 1;
            DatItem dfe = Next();
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
            Dat_reset(_dat);
            FuryException.Throw();
        }

        protected DatItem Next()
        {
            DatHeader header = new DatHeader();
            if (Dat_next(_dat, ref header))
            {
                return new DatItem(header, this, _index);
            }
            FuryException.Throw();
            return null;
        }

        public DatItem Item(int index)
        {
            CheckDisposed();
            DatHeader header = new DatHeader();
            if (Dat_header(_dat, (UInt32)index, ref header))
            {
                return new DatItem(header, this, (UInt32)index);
            }
            FuryException.Throw();
            return null;
        }

        public void Add(string fileName, byte[] buffer, bool compress)
        {
            CheckDisposed();
            byte[] fileNameBuffer = Encoding.ASCII.GetBytes(fileName);
            Dat_add(_dat, fileNameBuffer, buffer, buffer.Length, compress);
            FuryException.Throw();
        }

        public byte[] Buffer()
        {
            CheckDisposed();
            byte[] buffer = new byte[Dat_size(_dat)];
            if (Dat_buffer(_dat, buffer, buffer.Length))
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

            Dat_destroy(_dat);

            _disposed = true;
        }


    }
}
