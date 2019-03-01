using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using Task2;

namespace UnitTests
{
    [TestClass]
    public class KeyUnitTests
    {
        [TestMethod]
        public void TestCompareTo()
        {
            TestKey testKey1 = new TestKey(0, 0);
            TestKey testKey2 = new TestKey(1, 0);
            TestKey testKey3 = new TestKey(2, 0);

            TestKey testKey4 = new TestKey(0, 0);
            TestKey testKey5 = new TestKey(0, 1);
            TestKey testKey6 = new TestKey(0, 2);

            Assert.AreEqual(-1, testKey1.CompareTo(testKey2));
            Assert.AreEqual(1, testKey2.CompareTo(testKey1));
            Assert.AreEqual(-1, testKey1.CompareTo(testKey3));
            Assert.AreEqual(1, testKey3.CompareTo(testKey1));
            Assert.AreEqual(0, testKey1.CompareTo(testKey4));
            Assert.AreEqual(0, testKey4.CompareTo(testKey1));
            Assert.AreEqual(-1, testKey1.CompareTo(testKey5));
            Assert.AreEqual(1, testKey5.CompareTo(testKey1));
            Assert.AreEqual(-1, testKey1.CompareTo(testKey6));
            Assert.AreEqual(1, testKey6.CompareTo(testKey1));
        }
    }
}

