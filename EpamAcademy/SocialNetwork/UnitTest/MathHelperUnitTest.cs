using Data.Repositories;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace UnitTests
{
    [TestClass]
    public class MathHelperUnitTest
    {
        [TestMethod]
        public void GenerateUniqueNumberFromPair()
        {
            int result = MathHelpers.GenerateUniqueNumberFromPair(1, 7);
            Assert.AreEqual(37, result);
        }
    }
}
