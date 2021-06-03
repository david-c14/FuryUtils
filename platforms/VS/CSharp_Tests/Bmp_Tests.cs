//using Microsoft.VisualStudio.TestTools.UnitTesting;
//using carbon14.FuryUtils;
//using System;
//using System.Linq;

//namespace CSharp_Tests
//{
//    [TestClass]
//    public class Bmp_Tests
//    {
//        [TestMethod]
//        public void Given_an_invalid_bmp_file_When_a_bmp_is_created_Then_an_exception_is_raised()
//        {
//            try
//            {
//                Bmp bmp = new Bmp(Utils.ReadFile("badrle.bmp"));
//                Assert.Fail("Exception was not thrown");
//            }
//            catch (FuryException ex)
//            {
//                Assert.AreEqual(ErrorCodes.INVALID_FORMAT, ex.ErrorCode, "Incorrect error code was set");
//                Assert.AreEqual("Compressed data contains an error", ex.Message, "Incorrect error message was set");
//            }
//            catch (Exception ex)
//            {
//                Assert.Fail($"Incorrect exception: {ex.Message} was thrown");
//            }
//        }

//        [TestMethod]
//        public void Given_an_invalid_imm_file_When_a_bmp_is_created_Then_an_exception_is_raised()
//        {
//            try
//            {
//                Bmp bmp = new Bmp(Utils.ReadFile("tooshort.bmp"), Utils.ReadFile("pal8out.pam"));
//                Assert.Fail("Exception was not thrown");
//            }
//            catch (FuryException ex)
//            {
//                Assert.AreEqual(ErrorCodes.INVALID_FORMAT, ex.ErrorCode, "Incorrect error code was set");
//                Assert.AreEqual("Image buffer size is too short for valid Imm", ex.Message, "Incorrect error message was set");
//            }
//            catch (Exception ex)
//            {
//                Assert.Fail($"Incorrect exception: {ex.Message} was thrown");
//            }
//        }

//        [TestMethod]
//        public void Given_a_valid_bmp_file_When_pixel_and_palette_buffers_are_requested_Then_the_correct_buffers_are_returned()
//        {
//            Bmp bmp = new Bmp(Utils.ReadFile("pal8out.bmp"));
//            byte[] expectedPixelBuffer = Utils.ReadFile("pal8out.imm");
//            byte[] expectedPaletteBuffer = Utils.ReadFile("pal8out.pam");
//            byte[] actualPixelBuffer = bmp.ImmBuffer;
//            byte[] actualPaletteBuffer = bmp.PamBuffer;
//            Assert.IsTrue(actualPixelBuffer.SequenceEqual(expectedPixelBuffer), "Imm buffer is not correct");
//            Assert.IsTrue(actualPaletteBuffer.SequenceEqual(expectedPaletteBuffer), "Pam buffer is not correct");
//        }

//        [TestMethod]
//        public void Given_a_valid_bmp_file_When_bmp_buffer_is_requested_Then_the_correct_buffer_is_returned()
//        {
//            Bmp bmp = new Bmp(Utils.ReadFile("pal8out.imm"), Utils.ReadFile("pal8out.pam"));
//            byte[] expectedBuffer = Utils.ReadFile("pal8qnt.bmp");
//            byte[] actualBuffer = bmp.Buffer;
//            Assert.IsTrue(actualBuffer.SequenceEqual(expectedBuffer), "Buffer is not correct");
//        }
//    }
//}
