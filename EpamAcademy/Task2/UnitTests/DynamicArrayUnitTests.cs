using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using Task2;

namespace UnitTests
{
    [TestClass]
    public class DynamicArrayUnitTests
    {
       
        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestCreation_InvalidCapacity1()
        {
            DynamicArray<int> array = new DynamicArray<int>(0);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestCreation_InvalidCapacity2()
        {
            DynamicArray<int> array = new DynamicArray<int>(-2);
        }

        [TestMethod]
        public void TestCreation_DefaultCapacity()
        {
            DynamicArray<int> array = new DynamicArray<int>();
            Assert.AreEqual(2, array.Capacity);
            Assert.AreEqual(0, array.Count);
        }

        [TestMethod]
        public void TestCreation_Capacity()
        {
            DynamicArray<int> array = new DynamicArray<int>(4);
            Assert.AreEqual(4, array.Capacity);
            Assert.AreEqual(0, array.Count);
        }

        [TestMethod]
        public void TestAddItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);

            array.Add(2);
            Assert.AreEqual(1, array.Count);
            Assert.AreEqual(2, array[0]);

            array.Add(3);
            Assert.AreEqual(2, array.Count);
            Assert.AreEqual(3, array[1]);

            array.Add(5);
            Assert.AreEqual(3, array.Count);
            Assert.AreEqual(5, array[2]);

            array.Add(7);
            Assert.AreEqual(4, array.Count);
            Assert.AreEqual(7, array[3]);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestInsert_InvalidIndex1()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Insert(-1, 3);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestInsert_InvalidIndex2()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Insert(3, 7);
        }

        [TestMethod]
        public void TestInsert_FirstItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(2);
            array.Add(5);
            array.Add(7);
            array.Insert(0, 3);

            Assert.AreEqual(4, array.Count);
            Assert.AreEqual(3, array[0]);
            Assert.AreEqual(2, array[1]);
        }

        [TestMethod]
        public void TestInsert_MiddleItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            array.Add(8);
            array.Add(5);
            array.Add(9);

            array.Insert(2, 3);
            Assert.AreEqual(5, array.Count);
            Assert.AreEqual(8, array[1]);
            Assert.AreEqual(3, array[2]);
            Assert.AreEqual(5, array[3]);
        }

        [TestMethod]
        public void TestInsert_LastItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            array.Add(8);
            array.Add(5);
            array.Add(9);

            array.Insert(4, 1);
            Assert.AreEqual(5, array.Count);
            Assert.AreEqual(9, array[3]);
            Assert.AreEqual(1, array[4]);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestRemove_InvalidIndex1()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.RemoveAt(-1);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestRemove_InvalidIndex2()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.RemoveAt(0);
        }

        [TestMethod]
        public void TestRemove_FirstItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            array.Add(8);
            array.Add(7);
            array.Add(6);
            array.Add(2);

            array.RemoveAt(0);
            Assert.AreEqual(4, array.Count);
            Assert.AreEqual(8, array[0]);
        }

        [TestMethod]
        public void TestRemove_MiddleItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            array.Add(8);
            array.Add(7);
            array.Add(6);
            array.Add(2);

            array.RemoveAt(2);
            Assert.AreEqual(4, array.Count);
            Assert.AreEqual(6, array[2]);
        }

        [TestMethod]
        public void TestRemove_LastItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            array.Add(8);
            array.Add(7);
            array.Add(6);
            array.Add(2);

            array.RemoveAt(4);
            Assert.AreEqual(4, array.Count);
            Assert.AreEqual(6, array[3]);
        }

        [TestMethod]
        public void TestRemoveItem1()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(2);
            array.Add(3);
            array.Add(5);
            array.Add(6);
            array.Add(8);
            array.Add(9);

            array.Remove(2);
            Assert.AreEqual(5, array.Count);
            Assert.AreEqual(3, array[0]);
        }

        [TestMethod]
        public void TestRemoveItem2()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(2);
            array.Add(3);
            array.Add(5);
            array.Add(6);
            array.Add(8);
            array.Add(9);

            array.Remove(6);
            Assert.AreEqual(5, array.Count);
            Assert.AreEqual(8, array[3]);
        }

        [TestMethod]
        public void TestRemoveItem3()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(2);
            array.Add(3);
            array.Add(5);
            array.Add(6);
            array.Add(8);
            array.Add(9);

            array.Remove(9);
            Assert.AreEqual(5, array.Count);
            Assert.AreEqual(8, array[4]);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestChangeItem_InvalidIndex1()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array[-6] = 5;
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestChangeItem_InvalidIndex2()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array[0] = 5;
        }

        [TestMethod]
        public void TestChangeItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(2);

            array[0] = 4;
            Assert.AreEqual(4, array[0]);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestGetItem_InvalidIndex1()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            Assert.AreNotEqual(4, array[-5]);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentOutOfRangeException), "Expected ArgumentOutOfRangeException")]
        public void TestGetItem_InvalidIndex2()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            Assert.AreNotEqual(4, array[5]);
        }

        [TestMethod]
        public void TestGetItem()
        {
            DynamicArray<int> array = new DynamicArray<int>(2);
            array.Add(4);
            Assert.AreEqual(4, array[0]);

            array.Add(8);
            Assert.AreEqual(8, array[1]);

            array.Add(3);
            Assert.AreEqual(3, array[2]);
        }

        [TestMethod]
        public void TestIndexOf_NotFound()
        {
            DynamicArray<int> array = new DynamicArray<int>(5);
            array.Add(4);
            array.Add(8);
            array.Add(2);
            array.Add(1);
            array.Add(7);

            Assert.AreEqual(-1, array.IndexOf(6));
        }

        [TestMethod]
        public void TestIndexOf_Found()
        {
            DynamicArray<int> array = new DynamicArray<int>(5);
            array.Add(4);
            array.Add(8);
            array.Add(9);
            array.Add(1);
            array.Add(7);

            Assert.AreEqual(2, array.IndexOf(9));
        }

    }
}
