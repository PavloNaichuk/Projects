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

    [TestClass]
    public class MatrixUnitTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestNullArgument()
        {
            Matrix matrix = new Matrix(null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestInvalidArgument1()
        {
            Matrix mat = new Matrix(0, 3);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestInvalidArgument2()
        {
            Matrix mat = new Matrix(2, 0);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestAdditionNullArgument1()
        {
            Matrix mat1 = null;
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat3 = mat1 + mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestAdditionNullArgument2()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = null;
            Matrix mat3 = mat1 + mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestSubtractionNullArgument1()
        {
            Matrix mat1 = null;
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat3 = mat1 - mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestSubtractionNullArgument2()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = null;
            Matrix mat3 = mat1 - mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestProductNullArgument1()
        {
            Matrix mat1 = null;
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat3 = mat1 * mat2;

        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestProductNullArgument2()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = null;
            Matrix mat3 = mat1 * mat2;
        }

        [TestMethod]
        public void TestAddition()
        {
            Matrix mat1 = new Matrix(new float[,] { { 2.0f, 1.0f, 2.0f }, { 3.0f, 4.0f, 3.0f }, { 4.0f, 1.0f, 2.0f } });
            Matrix mat2 = new Matrix(new float[,] { { 2.0f, 1.0f, 2.0f }, { 4.0f, 1.0f, 3.0f }, { 1.0f, 2.0f, 2.0f } });
            Matrix mat3 = mat1 + mat2;

            Matrix mat4 = new Matrix(new float[,] { { 4.0f, 2.0f, 4.0f }, { 7.0f, 5.0f, 6.0f }, { 5.0f, 3.0f, 4.0f } });
            Assert.IsTrue(Matrix.AreEqual(mat3, mat4));
        }

        [TestMethod]
        public void TestSubtraction()
        {
            Matrix mat1 = new Matrix(new float[,] { { 1.0f, 5.0f, 7.0f }, { 1.0f, 3.0f, 8.0f } });
            Matrix mat2 = new Matrix(new float[,] { { 3.0f, 4.0f, 2.0f }, { 5.0f, 1.0f, 8.0f } });
            Matrix mat3 = mat1 - mat2;

            Matrix mat4 = new Matrix(new float[,] { { -2.0f, 1.0f, 5.0f }, { -4.0f, 2.0f, 0 } });
            Assert.IsTrue(Matrix.AreEqual(mat3, mat4));
        }

        [TestMethod]
        public void TestProduct()
        {
            Matrix mat1 = new Matrix(new float[,] { { 3.0f, 2.0f }, { 1.0f, 7.0f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 2.0f }, { 5.0f, 2.0f } });
            Matrix mat3 = mat1 * mat2;

            Matrix mat4 = new Matrix(new float[,] { { 22.0f, 10.0f }, { 39.0f, 16.0f } });
            Assert.IsTrue(Matrix.AreEqual(mat3, mat4));
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestAdditionIncompartible()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f, 2.5f }, { 4.0f, 3.0f, 2.7f, 6.7f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat3 = mat1 + mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestSubtractionIncompartible()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f, 2.5f }, { 4.0f, 3.0f, 2.7f, 6.7f } });
            Matrix mat3 = mat1 - mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void TestProductIncompartible()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f, 2.5f }, { 4.0f, 3.0f, 2.7f, 6.7f } });
            Matrix mat3 = mat1 * mat2;
        }
    }
}
