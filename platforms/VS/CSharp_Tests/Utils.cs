using System.IO;

namespace CSharp_Tests
{
    public class Utils
    {
        public static byte[] ReadFile(string fileName)
        {
            byte[] buffer;
            using (FileStream fs = new FileStream($"..\\..\\..\\testassets\\{fileName}", FileMode.Open))
            {
                buffer = new byte[fs.Length];
                fs.Read(buffer, 0, buffer.Length);
            }
            return buffer;
        }
    }
}
