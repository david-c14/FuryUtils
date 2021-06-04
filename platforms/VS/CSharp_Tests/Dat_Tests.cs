using Microsoft.VisualStudio.TestTools.UnitTesting;
using carbon14.FuryUtils;
using System;
using System.Linq;

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
            foreach (Dat.DatItem item in df)
            {
                Assert.AreEqual(count, item.Index, "Index is not correct");
                Assert.IsTrue(item.IsCompressed, "Compression flag is not correct");
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
            Assert.IsTrue(item.IsCompressed, "Compression flag is not correct");
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

        [TestMethod]
        public void Given_an_empty_dat_Then_count_returns_correct_value()
        {
            Dat df = new Dat();
            Assert.AreEqual(0, df.Count, "Incorrect count was returned");
        }

        [TestMethod]
        public void Given_an_empty_dat_When_files_are_added_Then_count_returns_correct_value()
        {
            Dat df = new Dat();
            df.Add("pal8out.bmp", Utils.ReadFile("pal8out.bmp"), true);
            df.Add("pal4out.bmp", Utils.ReadFile("pal4out.bmp"), false);
            Assert.AreEqual(2, df.Count, "Incorrect count was returned");
        }

        [TestMethod]
        public void Given_an_empty_dat_When_compressed_files_are_added_Then_returned_buffer_is_correct()
        {
            Dat df = new Dat();
            df.Add("pal8out.bmp", Utils.ReadFile("pal8out.bmp"), true);
            df.Add("pal4out.bmp", Utils.ReadFile("pal4out.bmp"), true);
            byte[] actual = df.Buffer;
            byte[] expected = Utils.ReadFile("basic.dat");
            Assert.IsTrue(actual.SequenceEqual(expected), "Buffer differs");
        }

        [TestMethod]
        public void Given_an_empty_dat_When_uncompressed_files_are_added_Then_buffer_size_is_correct()
        {
            Dat df = new Dat();
            byte[] pal8 = Utils.ReadFile("pal8out.bmp");
            df.Add("pal8out.bmp", pal8, false);
            byte[] actual = df.Buffer;
            Assert.AreEqual(pal8.Length + 24, actual.Length, "Buffer length is incorrect");
        }

        [TestMethod]
        public void Given_an_empty_dat_When_files_are_added_Then_they_can_be_correctly_retrieved()
        {
            byte[] pal8 = Utils.ReadFile("pal8out.bmp");
            byte[] pal4 = Utils.ReadFile("pal4out.bmp");
            Dat df = new Dat();
            df.Add("pal8out.bmp", pal8, true);
            df.Add("pal4out.bmp", pal4, false);
            uint count = 0;
            foreach (Dat.DatItem item in df)
            {
                Assert.AreEqual(count, item.Index, "Index is not correct");
                switch (count)
                {
                    case 0:
                        Assert.IsTrue(item.IsCompressed, "Compression flag is not correct");
                        Assert.AreEqual(4767u, item.CompressedSize, "Compressed size is not correct");
                        Assert.AreEqual(9270u, item.UncompressedSize, "Uncompressed size is not correct");
                        Assert.AreEqual("pal8out.bmp", item.FileName, "Filename is not correct");
                        Assert.IsTrue(item.Buffer.SequenceEqual(pal8), "Buffer is not correct");
                        break;
                    case 1:
                        Assert.IsFalse(item.IsCompressed, "Compression flag is not correct");
                        Assert.AreEqual(4214u, item.CompressedSize, "Compressed size is not correct");
                        Assert.AreEqual(4214u, item.UncompressedSize, "Uncompressed size is not correct");
                        Assert.AreEqual("pal4out.bmp", item.FileName, "Filename is not correct");
                        Assert.IsTrue(item.Buffer.SequenceEqual(pal4), "Buffer is not correct");
                        break;
                    default:
                        Assert.Fail("Too many iterations");
                        break;
                }
                count++;
            }
        }
    }
}
