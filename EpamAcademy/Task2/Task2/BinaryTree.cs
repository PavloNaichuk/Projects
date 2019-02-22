using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace Task2
{
    public class BinaryTree<Key, Value>
        where Key : IComparable
    {
        private Node<Key, Value> mRootNode;
        private int mNodeCount;

        public BinaryTree()
        {
            mRootNode = null;
            mNodeCount = 0;
        }

        public int NodeCount
        {
            get { return mNodeCount; }
        }

        public bool InsertNode(Key key, Value value)
        {
            if (key == null)
                throw new ArgumentNullException("key", "Parameter is null");
            if (value == null)
                throw new ArgumentNullException("value", "Parameter is null");

            Node<Key, Value> parentNode = null;
            int compResult = 0;
            for (Node<Key, Value> currentNode = mRootNode; currentNode != null; )
            {
                compResult = key.CompareTo(currentNode.mKey);

                parentNode = currentNode;
                if (compResult > 0)    
                    currentNode = currentNode.mRightNode;
                else if (compResult < 0)
                    currentNode = currentNode.mLeftNode;
                else
                    currentNode = null;
            }

            if ((parentNode != null) && (compResult == 0))
            {
                /*Key already exists. Simply return. No duplicates allowed.*/
                return false;
            }

            Node<Key, Value> newNode = new Node<Key, Value>(key, value, parentNode);
            ++mNodeCount;

            if (parentNode == null)
                mRootNode = newNode;
            else if (compResult > 0)
                parentNode.mRightNode = newNode;
            else
                parentNode.mLeftNode = newNode;

            return true;
        }

        public Node<Key, Value> FindNode(Key key)
        {
            if (key == null)
                throw new ArgumentNullException("key", "Parameter is null");

            Node<Key, Value> currentNode = mRootNode;
            while (currentNode != null)
            {
                int compResult = key.CompareTo(currentNode.mKey);
                if (compResult > 0)
                    currentNode = currentNode.mRightNode;
                else if (compResult < 0)
                    currentNode = currentNode.mLeftNode;
                else
                    break;
            }
            return currentNode;
        }

        public void RemoveNode(Node<Key, Value> node)
        {
            if (node == null)
                return;

            --mNodeCount;

            if (node.mLeftNode == null)
                ReplaceNode(node, node.mRightNode);
            else if (node.mRightNode == null)
                ReplaceNode(node, node.mLeftNode);
            else
            {
                Node<Key, Value> minNode = FindMinNode(node.mRightNode);
                if (minNode.mParentNode != node)
                {
                    ReplaceNode(minNode, minNode.mRightNode);
                    minNode.mRightNode = node.mRightNode;
                    minNode.mRightNode.mParentNode = minNode;
                }
                ReplaceNode(node, minNode);
                minNode.mLeftNode = node.mLeftNode;
                minNode.mLeftNode.mParentNode = minNode;
            }
        }

        public void RemoveNode(Key key)
        {
            RemoveNode(FindNode(key));
        }

        public override string ToString()
        {
            return BuildString(mRootNode);
        }

        private string BuildString(Node<Key, Value> node)
        {
            if (node == null)
                return "";

            string left = BuildString(node.mLeftNode);
            string current = node.ToString() + "\n";
            string right = BuildString(node.mRightNode);

            return left + current + right;
        }

        private Node<Key, Value> FindMinNode(Node<Key, Value> rootNode)
        {
            Node<Key, Value> currentNode = rootNode;
            while (currentNode.mLeftNode != null)
                currentNode = currentNode.mLeftNode;

            return currentNode;
        }

        private void ReplaceNode(Node<Key, Value> oldNode, Node<Key, Value> newNode)
        {
            if (oldNode.mParentNode == null)
                mRootNode = newNode;
            else if (oldNode == oldNode.mParentNode.mLeftNode)
                oldNode.mParentNode.mLeftNode = newNode;
            else
                oldNode.mParentNode.mRightNode = newNode;

            if (newNode != null)
                newNode.mParentNode = oldNode.mParentNode;
        }
    }

    public class Test
    {
        string mName;

        public Test(string name)
        {
            if (name == null)
                throw new ArgumentNullException("name", "Parameter is null");
            mName = name;
        }
    }

    public class CompletedTest
    {
        int mTestId;
        float mScore;

        public CompletedTest(int testId, float score)
        {
            if (testId == 0)
                throw new ArgumentException("Invalid parameter.The parameter should be greater than 0", "rowCount");

            if (score == 0)
                throw new ArgumentException("Invalid parameter.The parameter should be greater than 0", "rowCount");

            mTestId = testId;
            mScore = score;
        }
    }

    public class Student
    {
        string mName;
        List<CompletedTest> mCompletedTests;
        DateTime mDate;

        public Student(string name, List<CompletedTest> completedTests, DateTime date)
        {
            if (name == null)
                throw new ArgumentNullException("name", "Parameter is null");
            mName = name;
            mCompletedTests = completedTests;
            mDate = date;
        }
    }
}
