using System;

namespace Task2
{
    public class Student
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
                    throw new ArgumentNullException("value", "Parameter is null");
                mName = value;
            }
        }

        public override string ToString()
        {
            return $"Student name: {Name}";
        }
    }
}