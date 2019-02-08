using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MatrixAndPolynom
{
    class MultiMember
    {
        float[] mValues;
        int mDegree;
        public MultiMember(float[] values, int degree)
        {
            mValues = values;
            mDegree = degree;
        }
        
        public static MultiMember operator +(MultiMember multiMemb1, MultiMember multiMemb2)
        {
            int digree = multiMemb1.mDegree;
            float[] values = new float[digree + 1];

            MultiMember multiMemb3 = new MultiMember(values, digree);
            for (int index = 0; index < multiMemb1.mDegree + 1; ++index)
                multiMemb3.mValues[index] = multiMemb1.mValues[index] + multiMemb2.mValues[index];
            return multiMemb3;
        }

        public static MultiMember operator -(MultiMember multiMemb1, MultiMember multiMemb2)
        {
            int digree = multiMemb1.mDegree;
            float[] values = new float[digree + 1];

            MultiMember multiMemb3 = new MultiMember(values, digree);
            for (int index = 0; index < multiMemb1.mDegree + 1; ++index)
                multiMemb3.mValues[index] = multiMemb1.mValues[index] - multiMemb2.mValues[index];
            return multiMemb3;
        }

        public static MultiMember operator *(MultiMember multiMemb1, MultiMember multiMemb2)
        {
            int digree = multiMemb1.mDegree;
            float[] values = new float[digree + 1];

            MultiMember multiMemb3 = new MultiMember(values, digree);
            for (int index = 0; index < multiMemb1.mDegree + 1; ++index)
                multiMemb3.mValues[index] = multiMemb1.mValues[index] * multiMemb2.mValues[index];
            return multiMemb3;
        }
    }
}
