using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task2
{
    class Student
    {
        private string mName;
        public Student(string name)
        {
           Name = name;
        }

        public string Name
        {
            get { return mName; }
            set
            {
                if (value == null)
                    throw new ArgumentNullException("name", "Parameter is null");
                mName = value;
            }
        }
    }
}