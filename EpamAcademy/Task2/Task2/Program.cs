using System;

namespace Task2
{
    class Program
    {
        static void Main(string[] args)
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            
            binaryTree.Insert(20, 2);
            binaryTree.Insert(15, 6);
            binaryTree.Insert(24, 8);
            binaryTree.Insert(12, 5);
            binaryTree.Insert(17, 2);
           
    
            Console.WriteLine($"{ binaryTree.ToString()}");
            Console.ReadKey();
        }
    }
}
