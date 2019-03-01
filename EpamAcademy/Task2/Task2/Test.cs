using System;

namespace Task2
{
    public class Test
    {
        private string mName;

        public Test(string name)
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
            return $"Test name: {Name}";
        }
    }
}
