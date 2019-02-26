using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task2
{
    class TestResult
    {
        private DateTime mDate;
        private float mScore;

        public TestResult(DateTime date, float score)
        {
            Date = date;
            Score = score;
        }

        public DateTime Date
        {
            get { return mDate; }
            set
            {
                if (value == null)
                    throw new ArgumentNullException("mDate", "Parameter is null");

                mDate = value;
            }
        }

        public float Score
        {
            get { return mScore; }
            set
            {
                if (value < 0)
                    throw new ArgumentException("Invalid parameter.The parameter should be greater or equal 0", "mScore");
                mScore = value;
            }
        }

        public override string ToString()
        {
            return $"Test result: date {Date}, score {Score}";
        }
    }
}
