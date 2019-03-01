using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Task1;

namespace UnitTests
{
    [TestClass]
    public class PolynomialUnitTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestPolynomCreation1()
        {
            Polynomial pol = new Polynomial(null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestPolynomCreation2()
        {
            Polynomial pol = new Polynomial(new float[] { });
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestPolynomCreation3()
        {
            Polynomial pol = new Polynomial(-3);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestPolynomCreation4()
        {
            Polynomial pol = new Polynomial(0);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestAdditionNullArgument1()
        {
            Polynomial pol1 = null;
            Polynomial pol2 = new Polynomial(new float[] { 8, 2, -1, 8, 1 });
            Polynomial pol3 = pol1 + pol2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestAdditionNullArgument2()
        {
            Polynomial pol1 = new Polynomial(new float[] { 8, 3, -5, 0, 3 });
            Polynomial pol2 = null;
            Polynomial pol3 = pol1 + pol2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestSubtractionNullArgument1()
        {
            Polynomial pol1 = null;
            Polynomial pol2 = new Polynomial(new float[] { 6, 0, -1, 0, 1 });
            Polynomial pol3 = pol1 - pol2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestSubtractionNullArgument2()
        {
            Polynomial pol1 = new Polynomial(new float[] { 4, 3, -1, 0, 8 });
            Polynomial pol2 = null;
            Polynomial pol3 = pol1 - pol2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestProductNullArgument1()
        {
            Polynomial pol1 = null;
            Polynomial pol2 = new Polynomial(new float[] { 8, 2, -1, 8, 1 });
            Polynomial pol3 = pol1 * pol2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestProductNullArgument2()
        {
            Polynomial pol1 = new Polynomial(new float[] { 8, 3, -5, 0, 3 });
            Polynomial pol2 = null;
            Polynomial pol3 = pol1 * pol2;
        }

        [TestMethod]
        public void TestAddition1()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, -1, 3, 0, 1 });
            Polynomial pol2 = new Polynomial(new float[] { 6, 2, 2, 1 });
            Polynomial pol3 = pol1 + pol2;

            Polynomial pol4 = new Polynomial(new float[] { 7, 1, 5, 1, 1 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestAddition2()
        {
            Polynomial pol1 = new Polynomial(new float[] { 6, 2, 0, 2 });
            Polynomial pol2 = new Polynomial(new float[] { 1, -1, 3, 0, 1 });
            Polynomial pol3 = pol1 + pol2;

            Polynomial pol4 = new Polynomial(new float[] { 7, 1, 3, 2, 1 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestAddition3()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, -1, 3 });
            Polynomial pol2 = new Polynomial(new float[] { 6, 2, 4 });
            Polynomial pol3 = pol1 + pol2;

            Polynomial pol4 = new Polynomial(new float[] { 7, 1, 7 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestSubtraction1()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, -3, 2, 0, 1 });
            Polynomial pol2 = new Polynomial(new float[] { 6, 2, 4 });
            Polynomial pol3 = pol1 - pol2;

            Polynomial pol4 = new Polynomial(new float[] { -5, -5, -2, 0, 1 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestSubtraction2()
        {
            Polynomial pol1 = new Polynomial(new float[] { 6, 2, 0, 2 });
            Polynomial pol2 = new Polynomial(new float[] { 3, 5, 6, 3, 3 });
            Polynomial pol3 = pol1 - pol2;

            Polynomial pol4 = new Polynomial(new float[] { 3, -3, -6, -1, -3 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestSubtraction3()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, -1, 3 });
            Polynomial pol2 = new Polynomial(new float[] { 6, 2, 4 });
            Polynomial pol3 = pol1 - pol2;

            Polynomial pol4 = new Polynomial(new float[] { -5, -3, -1 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestProduct1()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, 3, 2 });
            Polynomial pol2 = new Polynomial(new float[] { -6, 2 });
            Polynomial pol3 = pol1 * pol2;

            Polynomial pol4 = new Polynomial(new float[] { -6, -16, -6, 4 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestProduct2()
        {
            Polynomial pol1 = new Polynomial(new float[] { -5, 0 });
            Polynomial pol2 = new Polynomial(new float[] { 1, 0, 2 });
            Polynomial pol3 = pol1 * pol2;

            Polynomial pol4 = new Polynomial(new float[] { -5, 0, -10});
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestProduct3()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, 4 });
            Polynomial pol2 = new Polynomial(new float[] { 2, 3 });
            Polynomial pol3 = pol1 * pol2;

            Polynomial pol4 = new Polynomial(new float[] { 2, 11, 12 });
            Assert.IsTrue(Polynomial.AreEqual(pol3, pol4));
        }

        [TestMethod]
        public void TestRemoveTrailingZeroCoefficients1()
        {
            Polynomial pol1 = new Polynomial(new float[] { 2, 4, 3, 0 });
            pol1.RemoveTrailingZeroCoefficients();

            Polynomial pol2 = new Polynomial(new float[] { 2, 4, 3 });
            Assert.IsTrue(Polynomial.AreEqual(pol1, pol2));
        }

        [TestMethod]
        public void TestRemoveTrailingZeroCoefficients2()
        {
            Polynomial pol1 = new Polynomial(new float[] { 0, 0, 0, 0 });
            pol1.RemoveTrailingZeroCoefficients();

            Polynomial pol2 = new Polynomial(new float[] { 0 });
            Assert.IsTrue(Polynomial.AreEqual(pol1, pol2));
        }

        [TestMethod]
        public void TestRemoveTrailingZeroCoefficients3()
        {
            Polynomial pol1 = new Polynomial(new float[] { 1, 2, 0, 0 });
            pol1.RemoveTrailingZeroCoefficients();

            Polynomial pol2 = new Polynomial(new float[] { 1, 2 });
            Assert.IsTrue(Polynomial.AreEqual(pol1, pol2));
        }

        [TestMethod]
        public void TestRemoveTrailingZeroCoefficients4()
        {
            Polynomial pol1 = new Polynomial(new float[] { 0 });
            pol1.RemoveTrailingZeroCoefficients();

            Polynomial pol2 = new Polynomial(new float[] { 0 });
            Assert.IsTrue(Polynomial.AreEqual(pol1, pol2));
        }
    }
}
