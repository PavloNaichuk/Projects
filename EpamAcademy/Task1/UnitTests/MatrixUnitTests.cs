using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Task1;

namespace UnitTests
{
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
        [ExpectedException(typeof(IncompartibleMatrixException), "Expected IncompartibleMatrixException")]
        public void TestAdditionIncompartible()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f, 2.5f }, { 4.0f, 3.0f, 2.7f, 6.7f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat3 = mat1 + mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(IncompartibleMatrixException), "Expected IncompartibleMatrixException")]
        public void TestSubtractionIncompartible()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f, 2.5f }, { 4.0f, 3.0f, 2.7f, 6.7f } });
            Matrix mat3 = mat1 - mat2;
        }

        [TestMethod]
        [ExpectedException(typeof(IncompartibleMatrixException), "Expected IncompartibleMatrixException")]
        public void TestProductIncompartible()
        {
            Matrix mat1 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } });
            Matrix mat2 = new Matrix(new float[,] { { 4.0f, 3.0f, 1.7f, 2.5f }, { 4.0f, 3.0f, 2.7f, 6.7f } });
            Matrix mat3 = mat1 * mat2;
        }
    }
}
