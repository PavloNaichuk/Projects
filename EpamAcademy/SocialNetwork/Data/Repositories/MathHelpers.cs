using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Data.Repositories
{
    public class MathHelpers
    {
        public static int GenerateUniqueNumberFromPair(int x, int y)
        {
            // Based on Cantor's Pairing Function 
            // http://szudzik.com/ElegantPairing.pdf
            return (x * x + 3 * x + 2 * x * y + y + y * y) / 2;
        }
    }
}
