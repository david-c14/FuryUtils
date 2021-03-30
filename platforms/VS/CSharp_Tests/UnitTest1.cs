using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using carbon14.FuryUtils;
using System.IO;

namespace CSharp_Tests
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
            byte[] buffer;
            using (FileStream fs = new FileStream("LANG.DAT", FileMode.Open))
            {
                buffer = new byte[fs.Length];
                fs.Read(buffer, 0, buffer.Length);
            }
            Dat df = new Dat(buffer);
            Assert.AreEqual(18, df.Count);

        }
    }
}
