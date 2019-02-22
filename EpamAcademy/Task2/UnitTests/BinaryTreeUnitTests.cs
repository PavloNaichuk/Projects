using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Task2;

namespace UnitTests
{
    [TestClass]
    public class BinaryTreeUnitTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestInsertNodeNullArgument1()
        {
            BinaryTree<string, int> binaryTree = new BinaryTree<string, int>();
            string str = null;
            binaryTree.InsertNode(str, 2);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestInsertNodeNullArgument2()
        {
            BinaryTree<string, string> binaryTree = new BinaryTree<string, string>();
            string str1 = "Pavlik";
            string str2 = null;
            binaryTree.InsertNode(str1, str2);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestFindNodeNullArgument()
        {
            BinaryTree<string, int> binaryTree = new BinaryTree<string, int>();
            string str = null;
            binaryTree.FindNode(str);
        }

        [TestMethod]
        public void TestInsertNodeNotPresent()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            Assert.AreEqual(0, binaryTree.NodeCount);

            Assert.IsTrue(binaryTree.InsertNode(20, 2));
            Assert.AreEqual(1, binaryTree.NodeCount);

            Assert.IsTrue(binaryTree.InsertNode(15, 7));
            Assert.AreEqual(2, binaryTree.NodeCount);

            Assert.IsTrue(binaryTree.InsertNode(25, 3));
            Assert.AreEqual(3, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestInsertNodeAlreadyPresent()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            Assert.AreEqual(0, binaryTree.NodeCount);

            Assert.IsTrue(binaryTree.InsertNode(20, 2));
            Assert.AreEqual(1, binaryTree.NodeCount);

            Assert.IsTrue(binaryTree.InsertNode(15, 7));
            Assert.AreEqual(2, binaryTree.NodeCount);

            Assert.IsFalse(binaryTree.InsertNode(20, 2));
            Assert.AreEqual(2, binaryTree.NodeCount);

            Assert.IsTrue(binaryTree.InsertNode(25, 8));
            Assert.AreEqual(3, binaryTree.NodeCount);

            Assert.IsFalse(binaryTree.InsertNode(15, 7));
            Assert.AreEqual(3, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemoveNodeNullArgument()
        {
            BinaryTree<string, string> binaryTree = new BinaryTree<string, string>();
            Node<string, string> node = new Node<string, string>(null, null, null);
            binaryTree.RemoveNode(node);
        }
    }
}
