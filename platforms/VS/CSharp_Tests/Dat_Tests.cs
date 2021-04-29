using Microsoft.VisualStudio.TestTools.UnitTesting;
using carbon14.FuryUtils;
using System.IO;
using System;

namespace CSharp_Tests
{
    [TestClass]
    public class Dat_Tests
    {
        [TestMethod]
        public void Given_a_valid_dat_file_When_a_DAT_object_is_created_Then_the_count_property_returns_a_count_of_content_files()
        {
            Dat df = new Dat(Utils.ReadFile("basic.dat"));
            Assert.AreEqual(2, df.Count);
        }

        [TestMethod]
        public void Given_a_valid_dat_file_When_iterated_Then_correct_headers_are_available()
        {
            uint count = 0;
            Dat df = new Dat(Utils.ReadFile("basic.dat"));
            foreach(Dat.DatItem item in df.Items)
            {
                Assert.AreEqual(count, item.Index, "Index is not correct");
                Assert.IsFalse(item.IsNotCompressed, "Compression flag is not correct");
                switch (count)
                {
                    case 0:
                        Assert.AreEqual(4767u, item.CompressedSize, "Compressed size is not correct");
                        Assert.AreEqual(9270u, item.UncompressedSize, "Uncompressed size is not correct");
                        Assert.AreEqual("pal8out.bmp", item.FileName, "Filename is not correct");
                        break;
                    case 1:
                        Assert.AreEqual(1698u, item.CompressedSize, "Compressed size is not correct");
                        Assert.AreEqual(4214u, item.UncompressedSize, "Uncompressed size is not correct");
                        Assert.AreEqual("pal4out.bmp", item.FileName, "Filename is not correct");
                        break;
                    default:
                        Assert.Fail("Too many iterations");
                        break;
                }
                count++;
            }
        }

        [TestMethod]
        public void Given_a_valid_dat_file_When_accessed_by_index_Then_the_correct_item_is_returned()
        {
            Dat df = new Dat(Utils.ReadFile("basic.dat"));
            Dat.DatItem item = df.Item(1);
            Assert.AreEqual(1698u, item.CompressedSize, "Compressed size is not correct");
            Assert.AreEqual(4214u, item.UncompressedSize, "Uncompressed size is not correct");
            Assert.AreEqual("pal4out.bmp", item.FileName, "Filename is not correct");
            Assert.AreEqual(1u, item.Index, "Index is not correct");
            Assert.IsFalse(item.IsNotCompressed, "Compression flag is not correct");

        }

        [TestMethod]
        public void Given_a_valid_dat_file_When_accessed_with_an_invalid_index_Then_INDEX_OUT_OF_RANGE_is_thrown()
        {
            Dat df = new Dat(Utils.ReadFile("basic.dat"));
            try
            {
                Dat.DatItem item = df.Item(2);
                Assert.Fail("Exception was not thrown");
            }
            catch (FuryException ex)
            {
                Assert.AreEqual(ErrorCodes.INDEX_OUT_OF_RANGE, ex.ErrorCode, "Incorrect error code was set");
                Assert.AreEqual("Index out of range", ex.Message, "Incorrect error message was set");
            }
            catch (Exception ex)
            {
                Assert.Fail($"Incorrect exception: {ex.Message} was thrown");
            }

        }
    }
}
