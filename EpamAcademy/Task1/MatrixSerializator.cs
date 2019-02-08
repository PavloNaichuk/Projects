using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;

namespace Task1
{
    class MatrixSerializator
    {
        public static void Serialize(string fileName, Matrix matrix)
        {
            IFormatter formatter = new BinaryFormatter();

            FileStream fileStream = new FileStream(fileName, FileMode.Create);
            formatter.Serialize(fileStream, matrix);
            fileStream.Close();
        }

        public static Matrix Deserialize(string fileName)
        {
            IFormatter formatter = new BinaryFormatter();

            FileStream fileStream = new FileStream(fileName, FileMode.Open);
            return (Matrix)formatter.Deserialize(fileStream);
        }
    }
}
