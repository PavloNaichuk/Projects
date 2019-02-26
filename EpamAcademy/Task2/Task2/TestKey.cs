using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task2
{
    class TestKey: IComparable
    {
        private int mTestId;
        private int mStudentId;

        TestKey(int testId, int studentId)
        {
            TestId = mTestId;
        }

        public int TestId
        {
            get { return mTestId; }
            set
            {
                if (value == 0)
                    throw new ArgumentException("Invalid parameter.The parameter should be greater than 0", "mTestId");

                mTestId = value;
            }
        }

        public int StudentId
        {
            get { return mStudentId; }
            set
            {
                if (value == 0)
                    throw new ArgumentException("Invalid parameter.The parameter should be greater than 0", "mStudentId");

                mStudentId = value;
            }
        }

        int CompareTo(object otherObject)
        {
            if (otherObject == null)
                throw new ArgumentNullException("otherObject", "Parameter is null");

            TestKey otherKey = otherObject as TestKey;
            if (otherKey == null)
                throw new ArgumentNullException("otherKey", "Parameter is null");

            if (mTestId > otherKey.mTestId)
                return 1;

            if (mTestId < otherKey.mTestId)
                return -1;

            if (mStudentId > otherKey.mStudentId)
                return 1;

            if (mStudentId < otherKey.mStudentId)
                return -1;
            
            return 0;
        }
    }
}
