using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using Task2;

namespace UnitTests
{
    [TestClass]
    public class TestUnitTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestTestNullArgument()
        {
            Test test = new Test(null);
        }

        [TestMethod]
        public void TestTestArgument()
        {
            Assert.AreEqual("Test name 1", new Test("Test name 1").Name);
        }
    }
}
