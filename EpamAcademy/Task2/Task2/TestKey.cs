using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task2
{
    public class TestKey: IComparable
    {
        private int mTestId;
        private int mStudentId;

        public TestKey(int testId, int studentId)
        {
            TestId = testId;
            StudentId = studentId;
        }

        public int TestId
        {
            get { return mTestId; }
            set
            {
                mTestId = value;
            }
        }

        public int StudentId
        {
            get { return mStudentId; }
            set
            {
                mStudentId = value;
            }
        }

        public int CompareTo(object otherObject)
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
