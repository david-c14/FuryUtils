using Microsoft.VisualStudio.TestTools.UnitTesting;
using carbon14.FuryUtils;
using System;
using System.Linq;

namespace CSharp_Tests
{
    [TestClass]
    public class Exceptions_Tests
    {
        [TestMethod]
        public void When_GetExceptionCode_is_called_Then_the_correct_code_is_returned()
        {
            Assert.AreEqual(0, FuryException.Code());
        }

        [TestMethod]
        public void When_Get_ExceptionString_is_called_Then_the_correct_string_is_returned()
        {
            Assert.AreEqual("", FuryException.ErrorString());
        }
    }
}
