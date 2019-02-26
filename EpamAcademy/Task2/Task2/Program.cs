using System;

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

                Test test1 = new Test("TestName1");
                Test test2 = new Test("TestName2");
                Test test3 = new Test("TestName3");

                Test[] tests = new Test[] { test1, test2, test3};

                Student student1 = new Student("StudentName1");
                Student student2 = new Student("StudentName2");
                Student student3 = new Student("StudentName3");

                Student[] students = new Student[] { student1, student2, student3 };

            }

            Console.ReadKey();
        }
    }
}
