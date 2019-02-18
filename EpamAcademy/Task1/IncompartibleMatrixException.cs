using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1
{
    class IncompartibleMatrixException : ArgumentException
    {
        public IncompartibleMatrixException(string message, string paramName) 
            : base(message)
        {
        }
    }
}
