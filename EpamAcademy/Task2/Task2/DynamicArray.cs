using System;

namespace Task2
{
    public class DynamicArray<T>
    {
        private T[] mItems;
        private int mCount;

        public DynamicArray()
            : this(2)
        {
        }

        public DynamicArray(int capacity)
        {
            if (capacity > 0)
            {
                mItems = new T[capacity];
                mCount = 0;
            }
            else
                throw new ArgumentOutOfRangeException("capacity", "Invalid capacity. The capacity should be greater than 0.");
        }

        public void Add(T item)
        {
            Insert(mCount, item);
        }

        public void Insert(int index, T item)
        {
            if ((index < 0) || (index > mCount))
                throw new ArgumentOutOfRangeException("index", "Invalid index. The index should be in the range from 0 to Count.");

            if (mCount == Capacity)
            {
                int newCapacity = 2 * Capacity;
                T[] newItems = new T[newCapacity];

                for (int i = 0; i < mCount; ++i)
                    newItems[i] = mItems[i];
                mItems = newItems;
            }

            for (int j = mCount; j > index; --j)
                mItems[j] = mItems[j - 1];
            mItems[index] = item;

            ++mCount;
        }

        public void RemoveAt(int index)
        {
            if ((index < 0) || (index > mCount - 1))
                throw new ArgumentOutOfRangeException("index", "Invalid index. The index should be in the range from 0 to Count - 1.");
            
            --mCount;
            for (int i = index; i < mCount; ++i)
                mItems[i] = mItems[i + 1];
        }

        public void Remove(T item)
        {
            RemoveAt(IndexOf(item));
        }

        public int IndexOf(T item)
        {
            for (int i = 0; i < Count; ++i)
            {
                if (mItems[i].Equals(item))
                    return i;
            }

            return -1;
        }

        public T this[int index]
        {
            get
            {
                if ((index < 0) || (index > mCount - 1))
                    throw new ArgumentOutOfRangeException("index", "Invalid index. The index should be in the range from 0 to Count - 1.");

                return mItems[index];
            }
            set
            {
                if ((index < 0) || (index > mCount - 1))
                    throw new ArgumentOutOfRangeException("index", "Invalid index. The index should be in the range from 0 to Count - 1.");

                mItems[index] = value;
            }
        }

        public int Count
        {
            get { return mCount; }
        }

        public int Capacity
        {
            get { return mItems.Length; }
        }
    }
}

