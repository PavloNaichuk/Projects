using System;

namespace Task2
{
    class Program
    {
        static void Main(string[] args)
        {
            //BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();

            //binaryTree.InsertNode(20, 2);
            //binaryTree.InsertNode(15, 6);
            //binaryTree.InsertNode(24, 8);
            //binaryTree.InsertNode(12, 5);
            //binaryTree.InsertNode(17, 2);

            //Console.WriteLine($"{ binaryTree.ToString()}");

            //Node<int, int> foundNode = binaryTree.FindNode(24);
            //Console.WriteLine($"Found node: { foundNode.ToString()}");

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
   
            Console.ReadKey();
        }
    }
}
