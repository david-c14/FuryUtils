using System;
using System.Collections.Generic;
using System.Text;

namespace carbon14.FuryUtils
{
    public static class ExceptionMessages
    {
        public static string Message(ErrorCodes exceptionCode)
        {
            switch (exceptionCode)
            {
                case ErrorCodes.NO_ERROR:
                    return "No Error";
                case ErrorCodes.BUFFER_OVERFLOW:
                    return "Buffer Overflow";
                case ErrorCodes.INDEX_OUT_OF_RANGE:
                    return "Index out of Range";
                case ErrorCodes.INVALID_FORMAT:
                    return "Invalid Format";
                default:
                    return "Unknown Exception";

            }
        }
    }
}
