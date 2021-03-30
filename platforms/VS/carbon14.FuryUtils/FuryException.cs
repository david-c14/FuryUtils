using System;
using System.Runtime.InteropServices;

namespace carbon14.FuryUtils
{
    public class FuryException : Exception
    {
        public FuryException(ErrorCodes errorCode) : this(errorCode, ExceptionMessages.Message(errorCode), null)
        {
        }

        public FuryException(ErrorCodes errorCode, string message): this(errorCode, message, null)
        {
        }

        public FuryException(ErrorCodes errorCode, string message, Exception innerException): base(message, innerException)
        {
            ErrorCode = errorCode;
        }

        public ErrorCodes ErrorCode { get; }

        [DllImport("FuryUtils.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int GetExceptionCode();

        public static int Code()
        {
            return GetExceptionCode();
        }

        public static void Throw()
        {
            int code = Code();
            if (code > 0)
            {
                throw new FuryException((ErrorCodes)code);
            }
        }
    }
}
