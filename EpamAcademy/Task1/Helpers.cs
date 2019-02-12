using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1
{
    class Helpers
    {
        public static bool AreEqual(float value1, float value2)
        {
            return (Math.Abs(value1 - value2) < 0.000001f);
        }
    }
}
