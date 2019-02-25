using System;
using System.Collections;
using System.Collections.Generic;
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
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestFindMinNodeNullArgument()
        {
            BinaryTree<string, int> binaryTree = new BinaryTree<string, int>();
            binaryTree.FindMinNode(null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void TestFindSuccessorNodeNullArgument()
        {
            BinaryTree<string, int> binaryTree = new BinaryTree<string, int>();
            binaryTree.FindSuccessorNode(null);
        }

        [TestMethod]
        public void TestInsertNodeNotPresent()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            Assert.AreEqual(0, binaryTree.NodeCount);
            Assert.IsNull(binaryTree.RootNode);

            Assert.IsTrue(binaryTree.InsertNode(20, 2));
            Assert.AreEqual(1, binaryTree.NodeCount);
            Assert.AreEqual(20, binaryTree.RootNode.mKey);

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
        public void TestEmptyTree()
        {
            BinaryTree<string, string> binaryTree = new BinaryTree<string, string>();
            Assert.AreEqual(0, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemoveNode_NoСhildren1()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(20, 2);
            binaryTree.InsertNode(15, 6);
            binaryTree.InsertNode(25, 6);
            Assert.AreEqual(20, binaryTree.RootNode.mKey);

            binaryTree.RemoveNode(15);
            Assert.IsNull(binaryTree.FindNode(15));
            Assert.AreEqual(2, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemoveNode_NoСhildren2()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(20, 2);
            binaryTree.InsertNode(15, 6);
            binaryTree.InsertNode(25, 4);
         
            binaryTree.RemoveNode(25);
            Assert.IsNull(binaryTree.FindNode(25));
            Assert.AreEqual(2, binaryTree.NodeCount);
        }


        [TestMethod]
        public void TestRemoveNode_NoRightChild()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(20, 2);
            binaryTree.InsertNode(15, 6);
            binaryTree.InsertNode(25, 8);
            binaryTree.InsertNode(10, 3);

            binaryTree.RemoveNode(15);
            Assert.IsNull(binaryTree.FindNode(15));
            Assert.AreEqual(3, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemoveNode_NoLeftChild()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(20, 2);
            binaryTree.InsertNode(15, 6);
            binaryTree.InsertNode(25, 8);
            binaryTree.InsertNode(28, 3);

            binaryTree.RemoveNode(25);
            Assert.IsNull(binaryTree.FindNode(25));
            Assert.AreEqual(3, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemove_TwoChildren1()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(2, 2);
            binaryTree.InsertNode(1, 6);
            binaryTree.InsertNode(4, 3);
            binaryTree.InsertNode(3, 8);
            binaryTree.InsertNode(5, 3);

            binaryTree.RemoveNode(2);
            Assert.IsNull(binaryTree.FindNode(2));
            Assert.AreEqual(4, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemove_TwoChildren2()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(2, 2);
            binaryTree.InsertNode(1, 6);
            binaryTree.InsertNode(4, 5);
            binaryTree.InsertNode(3, 8);
            binaryTree.InsertNode(5, 1);

            binaryTree.RemoveNode(4);
            Assert.IsNull(binaryTree.FindNode(4));
            Assert.AreEqual(4, binaryTree.NodeCount);
        }

        [TestMethod]
        public void TestRemove_AllChildren()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(2, 2);
            binaryTree.InsertNode(1, 6);
            binaryTree.InsertNode(4, 5);
            binaryTree.InsertNode(3, 8);
            binaryTree.InsertNode(5, 1);
            Assert.AreEqual(5, binaryTree.NodeCount);
            Assert.IsNotNull(binaryTree.RootNode);

            binaryTree.RemoveNode(2);
            binaryTree.RemoveNode(1);
            binaryTree.RemoveNode(4);
            binaryTree.RemoveNode(3);
            binaryTree.RemoveNode(5);
            Assert.AreEqual(0, binaryTree.NodeCount);
            Assert.IsNull(binaryTree.RootNode);
        }

        [TestMethod]
        public void TestFindMinNode()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(15, 1);
            binaryTree.InsertNode(10, 2);
            binaryTree.InsertNode(7, 3);
            binaryTree.InsertNode(12, 4);
            binaryTree.InsertNode(25, 5);
            binaryTree.InsertNode(20, 6);

            Assert.AreEqual(7, binaryTree.FindMinNode(binaryTree.FindNode(15)).mKey);
            Assert.AreEqual(7, binaryTree.FindMinNode(binaryTree.FindNode(10)).mKey);
            Assert.AreEqual(7, binaryTree.FindMinNode(binaryTree.FindNode(7)).mKey);
            Assert.AreEqual(12, binaryTree.FindMinNode(binaryTree.FindNode(12)).mKey);
            Assert.AreEqual(20, binaryTree.FindMinNode(binaryTree.FindNode(25)).mKey);
            Assert.AreEqual(20, binaryTree.FindMinNode(binaryTree.FindNode(20)).mKey);
        }

        [TestMethod]
        public void FindSuccessorNode()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(15, 1);
            binaryTree.InsertNode(10, 2);
            binaryTree.InsertNode(7, 3);
            binaryTree.InsertNode(12, 4);
            binaryTree.InsertNode(25, 5);
            binaryTree.InsertNode(20, 6);

            Assert.AreEqual(20, binaryTree.FindSuccessorNode(binaryTree.FindNode(15)).mKey);
            Assert.AreEqual(12, binaryTree.FindSuccessorNode(binaryTree.FindNode(10)).mKey);
            Assert.AreEqual(10, binaryTree.FindSuccessorNode(binaryTree.FindNode(7)).mKey);
            Assert.AreEqual(15, binaryTree.FindSuccessorNode(binaryTree.FindNode(12)).mKey);
            Assert.AreEqual(25, binaryTree.FindSuccessorNode(binaryTree.FindNode(20)).mKey);
            Assert.IsNull(binaryTree.FindSuccessorNode(binaryTree.FindNode(25)));
        }

        [TestMethod]
        public void TestEnumerator_EmptyTree()
        {
            BinaryTree<string, int> binaryTree = new BinaryTree<string, int>();

            IEnumerator<Node<string, int>> enumerator = binaryTree.GetEnumerator();
            Assert.IsFalse(enumerator.MoveNext());

            enumerator.Reset();
            Assert.IsFalse(enumerator.MoveNext());
        }

        [TestMethod]
        public void TestEnumerator_NotEmptyTree()
        {
            BinaryTree<int, int> binaryTree = new BinaryTree<int, int>();
            binaryTree.InsertNode(15, 1);
            binaryTree.InsertNode(10, 2);
            binaryTree.InsertNode(7, 3);

            IEnumerator<Node<int, int>> enumerator = binaryTree.GetEnumerator();

            Assert.IsTrue(enumerator.MoveNext());
            Assert.AreEqual(7, enumerator.Current.mKey);

            Assert.IsTrue(enumerator.MoveNext());
            Assert.AreEqual(10, enumerator.Current.mKey);

            Assert.IsTrue(enumerator.MoveNext());
            Assert.AreEqual(15, enumerator.Current.mKey);

            enumerator.Reset();
            Assert.IsTrue(enumerator.MoveNext());
            Assert.AreEqual(7, enumerator.Current.mKey);

        }
    }
}