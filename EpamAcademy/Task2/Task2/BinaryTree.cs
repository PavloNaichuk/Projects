using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace Task2
{
    public class BinaryTree<Key, Value> : IEnumerable<Node<Key, Value>>
        where Key : IComparable
    {
        private Node<Key, Value> mRootNode;
        private int mNodeCount;

        public BinaryTree()
        {
            mRootNode = null;
            mNodeCount = 0;
        }

        public Node<Key, Value> RootNode
        {
            get { return mRootNode; }
        }

        public int NodeCount
        {
            get { return mNodeCount; }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public IEnumerator<Node<Key, Value>> GetEnumerator()
        {
            return new NodeEnumerator(this);
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
                Node<Key, Value> successorNode = FindMinNode(node.mRightNode);
                if (successorNode.mParentNode != node)
                {
                    ReplaceNode(successorNode, successorNode.mRightNode);
                    successorNode.mRightNode = node.mRightNode;
                    successorNode.mRightNode.mParentNode = successorNode;
                }
                ReplaceNode(node, successorNode);
                successorNode.mLeftNode = node.mLeftNode;
                successorNode.mLeftNode.mParentNode = successorNode;
            }
        }

        public Node<Key, Value> FindMinNode(Node<Key, Value> rootNode)
        {
            if (rootNode == null)
                throw new ArgumentNullException("rootNode", "Parameter is null");

            Node<Key, Value> currentNode = rootNode;
            while (currentNode.mLeftNode != null)
                currentNode = currentNode.mLeftNode;

            return currentNode;
        }

        public Node<Key, Value> FindSuccessorNode(Node<Key, Value> node)
        {
            if (node == null)
                throw new ArgumentNullException("node", "Parameter is null");

            if (node.mRightNode != null)
                return FindMinNode(node.mRightNode);

            Node<Key, Value> successorNode = node.mParentNode;
            while ((successorNode != null) && (node == successorNode.mRightNode))
            {
                node = successorNode;
                successorNode = successorNode.mParentNode;
            }

            return successorNode;
        }

        public void RemoveNode(Key key)
        {
            RemoveNode(FindNode(key));
        }

        public override string ToString()
        {
            StringBuilder stringBuilder = new StringBuilder();

            foreach (Node<Key, Value> node in this)
                stringBuilder.AppendLine(node.ToString());

            return stringBuilder.ToString();
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

        private class NodeEnumerator : IEnumerator<Node<Key, Value>>
        {
            private BinaryTree<Key, Value> mTree;
            private Node<Key, Value> mCurrentNode;
            private bool mHasBeenReset;

            public NodeEnumerator(BinaryTree<Key, Value> tree)
            {
                mTree = tree;
                Reset();
            }

            object IEnumerator.Current
            {
                get { return mCurrentNode; }
            }

            public Node<Key, Value> Current
            {
                get { return mCurrentNode; }
            }

            public bool MoveNext()
            {
                if (mHasBeenReset)
                {
                    if (mTree.RootNode != null)
                        mCurrentNode = mTree.FindMinNode(mTree.RootNode);

                    mHasBeenReset = false;
                }
                else if (mCurrentNode != null)
                {
                    mCurrentNode = mTree.FindSuccessorNode(mCurrentNode);
                }
                return (mCurrentNode != null);
            }

            public void Reset()
            {
                mCurrentNode = null;
                mHasBeenReset = true;
            }

            void IDisposable.Dispose()
            {
            }
        }
    }
}
