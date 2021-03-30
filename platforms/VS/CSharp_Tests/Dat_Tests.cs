using Microsoft.VisualStudio.TestTools.UnitTesting;
using carbon14.FuryUtils;
using System.IO;

namespace CSharp_Tests
{
    [TestClass]
    public class Dat_Tests
    {
        [TestMethod]
        public void Given_a_valid_dat_file_When_a_DAT_object_is_created_Then_the_count_property_returns_a_count_of_content_files()
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
