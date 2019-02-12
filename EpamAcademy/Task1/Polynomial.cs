using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task1
{
    public class Polynomial
    {
        float[] mCoeffs;

        public Polynomial(float[] coeffs)
        {
            if (coeffs == null)
                throw new ArgumentNullException("coeffs", "Parameter is null");

            mCoeffs = new float[coeffs.Length];
            for (int index = 0; index < coeffs.Length; ++index)
                mCoeffs[index] = coeffs[index];
        }

        public Polynomial(int coeffCount)
        {
            mCoeffs = new float[coeffCount];
            for (int index = 0; index < coeffCount; ++index)
                mCoeffs[index] = 0;
        }

        public int Degree
        {
            get { return CoefficientCount - 1; }
        }

        public int CoefficientCount
        {
            get { return mCoeffs.Length; }
        }

        public float this[int index]
        {
            get { return mCoeffs[index]; }
            set { mCoeffs[index] = value; }
        }

        public static Polynomial operator +(Polynomial pol1, Polynomial pol2)
        {
            if (pol1 == null)
                throw new ArgumentNullException("pol1", "Parameter is null");

            if (pol2 == null)
                throw new ArgumentNullException("pol2", "Parameter is null");

            int maxCoeffCount = Math.Max(pol1.CoefficientCount, pol2.CoefficientCount);
            Polynomial pol = new Polynomial(maxCoeffCount);

            for (int index = 0; index < pol1.CoefficientCount; ++index)
                pol[index] = pol1[index];
            
            for (int index = 0; index < pol2.CoefficientCount; ++index)
                pol[index] += pol2[index];

            return pol;
        }

        public static Polynomial operator -(Polynomial pol1, Polynomial pol2)
        {
            if (pol1 == null)
                throw new ArgumentNullException("pol1", "Parameter is null");

            if (pol2 == null)
                throw new ArgumentNullException("pol2", "Parameter is null");

            int maxCoeffCount = Math.Max(pol1.CoefficientCount, pol2.CoefficientCount);
            Polynomial pol = new Polynomial(maxCoeffCount);

            for (int index = 0; index < pol1.CoefficientCount; ++index)
                pol[index] = pol1[index];

            for (int index = 0; index < pol2.CoefficientCount; ++index)
                pol[index] -= pol2[index];

            return pol;
        }

        public static Polynomial operator *(Polynomial pol1, Polynomial pol2)
        {
            if (pol1 == null)
                throw new ArgumentNullException("pol1", "Parameter is null");

            if (pol2 == null)
                throw new ArgumentNullException("pol2", "Parameter is null");

            Polynomial pol = new Polynomial(pol1.Degree + pol2.Degree + 1);

            for (int index1 = 0; index1 < pol1.CoefficientCount; ++index1)
            {
                for (int index2 = 0; index2 < pol2.CoefficientCount; ++index2)
                    pol[index1 + index2] += pol1[index1] * pol2[index2];
            }
            return pol;
        }

        public override string ToString()
        {
            return string.Format("Coefficients:*" + string.Join(";*", mCoeffs));
        }

        public static bool AreEqual(Polynomial pol1, Polynomial pol2)
        {
            if (pol1.CoefficientCount != pol2.CoefficientCount)
                return false;

            for (int index = 0; index < pol1.CoefficientCount; ++index)
            {
                if (!Helpers.AreEqual(pol1[index], pol2[index]))
                    return false;
            }
            return true;
        }
    }
}
