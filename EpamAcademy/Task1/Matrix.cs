using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Task1
{
    [Serializable]
    public class Matrix : ISerializable
    {
        private float[,] mValues;
        private int mRowCount;
        private int mColumnCount;

        public Matrix(float[,] values)
        {
            if(values == null)
                throw new ArgumentNullException("values", "Parameter is null");

            mRowCount = values.GetLength(0);
            mColumnCount = values.GetLength(1);
            mValues = new float[mRowCount, mColumnCount];
            for (int row = 0; row < mRowCount; ++row)
            {
                for (int column = 0; column < mColumnCount; ++column)
                    mValues[row, column] = values[row, column];
            }
        }

        public Matrix(int rowCount, int columnCount)
        {
            if (rowCount <= 0)
                throw new ArgumentException("Invalid parameter. The parameter should be greater than 0", "rowCount");

            if (columnCount <= 0)
                throw new ArgumentException("Invalid parameter. The parameter should be greater than 0", "columnCount");

            mValues = new float[rowCount, columnCount];
            for (int row = 0; row < rowCount; ++row)
            {
                for (int column = 0; column < columnCount; ++column)
                    mValues[row, column] = 0;
            }

            mRowCount = rowCount;
            mColumnCount = columnCount;
        }

        public Matrix(int dimension)
            :this(dimension, dimension)
        {
        }

        public Matrix(SerializationInfo info, StreamingContext context)
        {
            mRowCount = info.GetInt32("mRowCount");
            mColumnCount = info.GetInt32("mColumnCount");
            mValues = (float[,])info.GetValue("mValues", typeof(float[,]));
        }

        public int RowCount
        {
            get { return mRowCount; }
        }

        public int ColumnCount
        {
            get { return mColumnCount; }
        }

        public float GetValue(int row, int column)
        {
            return mValues[row, column];
        }

        public void SetValue(int row, int column, float value)
        {
            mValues[row, column] = value;
        }

        public static Matrix operator +(Matrix mat1, Matrix mat2)
        {
            if (mat1 == null)
                throw new ArgumentNullException("mat1", "Parameter is null");

            if (mat2 == null)
                throw new ArgumentNullException("mat2", "Parameter is null");

            if ((mat1.mColumnCount != mat2.mColumnCount) || (mat1.mRowCount != mat2.mRowCount))
                throw new ArgumentException("The matrix is incompartible", "mat2");

            Matrix newMat = new Matrix(mat1.mRowCount, mat1.mColumnCount);
            for (int row = 0; row < mat1.mRowCount; ++row)
            {
                for (int column = 0; column < mat1.mColumnCount; ++column)
                    newMat.mValues[row, column] = mat1.mValues[row, column] + mat2.mValues[row, column];
            }
            return newMat;
        }

        public static Matrix operator -(Matrix mat1, Matrix mat2)
        {
            if (mat1 == null)
                throw new ArgumentNullException("mat1", "Parameter is null");

            if (mat2 == null)
                throw new ArgumentNullException("mat2", "Parameter is null");

            if ((mat1.mColumnCount != mat2.mColumnCount) || (mat1.mRowCount != mat2.mRowCount))
                throw new ArgumentException("The matrix is incompartible", "mat2");

            Matrix newMat = new Matrix(mat1.mRowCount, mat1.mColumnCount);
            for (int row = 0; row < mat1.mRowCount; ++row)
            {
                for (int column = 0; column < mat1.mColumnCount; ++column)
                    newMat.mValues[row, column] = mat1.mValues[row, column] - mat2.mValues[row, column];
            }
            return newMat;
        }

        public static Matrix operator *(Matrix mat, float scalar)
        {
            if (mat == null)
                throw new ArgumentNullException("mat", "Parameter is null");

            Matrix newMat = new Matrix(mat.mRowCount, mat.mColumnCount);
            for (int row = 0; row < mat.mRowCount; ++row)
            {
                for (int column = 0; column < mat.mColumnCount; ++column)
                    newMat.mValues[row, column] = mat.mValues[row, column] * scalar;
            }
            return newMat;
        }

        public static Matrix operator *(float scalar, Matrix mat)
        {
            return mat * scalar;
        }

        public static Matrix operator *(Matrix mat1, Matrix mat2)
        {
            if (mat1 == null)
                throw new ArgumentNullException("mat1", "Parameter is null");

            if (mat2 == null)
                throw new ArgumentNullException("mat2", "Parameter is null");

            if (mat1.mColumnCount != mat2.mRowCount)
                throw new ArgumentException("The matrix is incompartible", "mat2");

            Matrix newMat = new Matrix(mat1.mRowCount, mat2.mColumnCount);
            for (int row = 0; row < mat1.mRowCount; ++row)
             {
                 for (int column = 0; column < mat1.mColumnCount; ++column)
                 {
                    float rezult = 0;
                    for (int index = 0; index < newMat.mRowCount; ++index)
                        rezult += mat1.mValues[row, index] * mat2.mValues[index, column];

                    newMat.mValues[row, column] = rezult;
                 }
             }
             return newMat;
        }

        public override string ToString()
        {
            var stringBuilder = new StringBuilder();

            for (int row = 0; row < mRowCount; ++row)
            {
                for (int column = 0; column <mColumnCount; ++column)
                    stringBuilder.Append($"{this.mValues[row, column]} ");

                stringBuilder.AppendLine();
            }

            return stringBuilder.ToString();
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("mRowCount", mRowCount);
            info.AddValue("mColumnCount", mColumnCount);
            info.AddValue("mValues", mValues, typeof(float[,]));
        }
    }
}

