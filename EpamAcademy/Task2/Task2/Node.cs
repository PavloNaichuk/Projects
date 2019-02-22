using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task2
{
    public class Node<Key, Value>
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
            return string.Format("[pk:{0}, k:{1}, v:{2}, lk:{3}, rk:{4}]",
                 (mParentNode != null) ? mParentNode.mKey.ToString() : "null",
                 mKey.ToString(),
                 mValue.ToString(),
                 (mLeftNode != null) ? mLeftNode.mKey.ToString() : "null",
                 (mRightNode != null) ? mRightNode.mKey.ToString() : "null");
        }
    }
}
