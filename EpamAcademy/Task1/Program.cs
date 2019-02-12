using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1
{
    class Program
    {
        static void Main(string[] args)
        {
            float[,] mas1 = { { 4.0f, 3.0f, 1.7f }, { 4.0f, 3.0f, 2.7f } };
            float[,] mas2 = { { 2.0f, 2.0f, 4.2f }, { 3.0f, 3.0f, 6.2f } };

            Matrix matrix1 = new Matrix(mas1);
            Matrix matrix2 = new Matrix(mas2);

            Matrix matplus = matrix1 + matrix2;

            Console.WriteLine($"MatPlus:{matplus}");

            MatrixSerializator.Serialize("matrix.dat", matrix1);
            Console.WriteLine($"RowCount:{matrix1.RowCount}");
            Console.WriteLine($"ColumnCount:{matrix1.ColumnCount}");

            Matrix matrix3 = MatrixSerializator.Deserialize("matrix.dat");
            Console.WriteLine($"RowCount:{matrix3.RowCount}");
            Console.WriteLine($"ColumnCount:{matrix3.ColumnCount}");

            Console.WriteLine($"matrix1:{matrix1}");
            Console.WriteLine($"matrix3:{matrix3}");
            Console.ReadKey();
        }
    }
}
