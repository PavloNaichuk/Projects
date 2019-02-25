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
            get { return Date; }
            set
            {
                if (value == null)
                    throw new ArgumentNullException("date", "Parameter is null");

                mDate = value;
            }
        }

        public float Score
        {
            get { return Score; }
            set
            {
                if (value < 0)
                    throw new ArgumentException("Invalid parameter.The parameter should be greater or equal 0", "score");
                Score = value;
            }
        }
    }
}
