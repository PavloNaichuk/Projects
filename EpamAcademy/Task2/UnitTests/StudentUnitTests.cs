using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using Task2;

namespace UnitTests
{
    [TestClass]
    public class StudentUnitTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestStudentNullArgument()
        {
            Student student = new Student(null);
        }

        [TestMethod]
        public void TestStudentArgument()
        {
            Assert.AreEqual("Student name 1", new Test("Student name 1").Name);
        }
    }
}
