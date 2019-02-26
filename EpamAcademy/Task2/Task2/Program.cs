using System;
using System.Text;

namespace Task2
{
    class Program
    {
        static void Main(string[] args)
        {
            {
                Console.WriteLine($"Test 1");
                BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();

                binaryTree.InsertNode(20, 2);
                Console.WriteLine($"Before:\n{ binaryTree.ToString()}");

                binaryTree.RemoveNode(20);
                Console.WriteLine($"After:\n{ binaryTree.ToString()}");
            }

            {
                Console.WriteLine($"Test 2");
                BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();

                binaryTree.InsertNode(20, 2);
                binaryTree.InsertNode(15, 6);
                Console.WriteLine($"Before:\n{ binaryTree.ToString()}");

                binaryTree.RemoveNode(15);
                Console.WriteLine($"After:\n{ binaryTree.ToString()}");
            }

            {
                Console.WriteLine($"Test 3");
                BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();

                binaryTree.InsertNode(20, 2);
                binaryTree.InsertNode(15, 6);
                Console.WriteLine($"Before:\n{ binaryTree.ToString()}");

                binaryTree.RemoveNode(20);
                Console.WriteLine($"After:\n{ binaryTree.ToString()}");
            }

            {
                Console.WriteLine($"Test 4");
                BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();

                binaryTree.InsertNode(2, 2);
                binaryTree.InsertNode(1, 6);
                binaryTree.InsertNode(4, 7);
                binaryTree.InsertNode(3, 7);
                binaryTree.InsertNode(5, 7);
                Console.WriteLine($"Before:\n{ binaryTree.ToString()}");

                binaryTree.RemoveNode(2);
                Console.WriteLine($"After:\n{ binaryTree.ToString()}");
            }

            {
                Console.WriteLine($"Test 5");
                BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();

                binaryTree.InsertNode(2, 2);
                binaryTree.InsertNode(1, 6);
                binaryTree.InsertNode(4, 7);
                binaryTree.InsertNode(3, 7);
                binaryTree.InsertNode(5, 7);
                Console.WriteLine($"Before:\n{ binaryTree.ToString()}");

                binaryTree.RemoveNode(4);
                Console.WriteLine($"After:\n{ binaryTree.ToString()}");
            }

            {
                Test[] tests = new Test[] 
                {
                    new Test("Test Name 1"),
                    new Test("Test Name 2"),
                    new Test("Test Name 3")
                };

                Student[] students = new Student[]
                {
                    new Student("Student Name 1"),
                    new Student("Student Name 2"),
                    new Student("Student Name 3"),
                    new Student("Student Name 4"),
                    new Student("Student Name 5"),
                    new Student("Student Name 6")
                };

                BinaryTree<TestKey, TestResult> binaryTree = new BinaryTree<TestKey, TestResult>();
                binaryTree.InsertNode(new TestKey(0, 0), new TestResult(new DateTime(2019, 2, 18), 6.2f));
                binaryTree.InsertNode(new TestKey(1, 1), new TestResult(new DateTime(2019, 1, 11), 7.8f));
                binaryTree.InsertNode(new TestKey(1, 0), new TestResult(new DateTime(2019, 1, 9), 9.2f));
                binaryTree.InsertNode(new TestKey(2, 0), new TestResult(new DateTime(2018, 12, 11), 8.9f));
                binaryTree.InsertNode(new TestKey(2, 2), new TestResult(new DateTime(2019, 2, 2), 11.0f));
                binaryTree.InsertNode(new TestKey(1, 2), new TestResult(new DateTime(2019, 1, 15), 9.7f));
                binaryTree.InsertNode(new TestKey(1, 3), new TestResult(new DateTime(2019, 2, 10), 2.8f));
                binaryTree.InsertNode(new TestKey(1, 5), new TestResult(new DateTime(2019, 2, 1), 8.4f));
                binaryTree.InsertNode(new TestKey(2, 3), new TestResult(new DateTime(2019, 1, 12), 11.7f));

                foreach (Node<TestKey, TestResult> node in binaryTree)
                {
                    Console.WriteLine(tests[node.mKey.TestId]);
                    Console.WriteLine(students[node.mKey.StudentId]);
                    Console.WriteLine(node.mValue);
                    Console.WriteLine();
                }
            }

            Console.ReadKey();
        }
    }
}
