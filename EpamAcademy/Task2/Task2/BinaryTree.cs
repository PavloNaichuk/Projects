using System;
using System.Collections.Generic;

namespace Task2
{
    public class BinaryTree<Key, Value>
        where Key : IComparable
    {
        private Node<Key, Value> mRootNode;

        public BinaryTree()
        {
            mRootNode = null;
        }

        public bool Insert(Key key, Value value)
        {
            if (key == null)
                throw new ArgumentNullException("key", "Parameter is null");
            if (value == null)
                throw new ArgumentNullException("value", "Parameter is null");

            if (mRootNode == null)
            {
                mRootNode = new Node<Key, Value>(key, value, null);
                return true;
            }

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

            if (compResult > 0)
            {
                parentNode.mRightNode = new Node<Key, Value>(key, value, parentNode);
                return true;
            }
            if (compResult < 0)
            {
                parentNode.mLeftNode = new Node<Key, Value>(key, value, parentNode);
                return true;
            }

            /*Key already exists. Simply return. No duplicates allowed.*/
            return false;
        }

        public void Remove(Key key, Value value)
        {
            if (key == null)
                throw new ArgumentNullException("key", "Parameter is null");
            if (value == null)
                throw new ArgumentNullException("value", "Parameter is null");

            if (mRootNode == null)
            {
                return;
            }

            Node<Key, Value> currentNode = mRootNode;
            Node<Key, Value> parentNode = currentNode;

            int compResult = key.CompareTo(currentNode.mKey);
            while (currentNode != null)
            {
                if (compResult > 0)
                {
                    
                }
                else if (compResult < 0)
                {
                    
                }
                else
                {
                    currentNode = null;
                }
            }

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

        public override string ToString()
        {
            return BuildString(mRootNode);
        }

        private class Node<Key, Value>
        {
            public Key mKey;
            public Value mValue;
            public Node<Key, Value> mParentNode;
            public Node<Key, Value> mLeftNode;
            public Node<Key, Value> mRightNode;

            public Node(Key key, Value value, Node<Key, Value> parentNode, Node<Key, Value> leftNode, Node<Key, Value> rightNode)
            {
                mKey = key;
                mValue = value;
                mParentNode = parentNode;
                mLeftNode = leftNode;
                mRightNode = rightNode;
            }

            public Node(Key key, Value value, Node<Key, Value> parentNode)
                : this(key, value, parentNode, null, null)
            {
            }

            public override string ToString()
            {
                return string.Format("[pk:{0}, k:{1}, lk:{2}, rk:{3}]",
                     (mParentNode != null) ? mParentNode.mKey.ToString() : "null",
                     mKey.ToString(),
                     (mLeftNode != null) ? mLeftNode.mKey.ToString() : "null",
                     (mRightNode != null) ? mRightNode.mKey.ToString() : "null");
            }
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
