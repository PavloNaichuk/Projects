using Microsoft.VisualStudio.TestTools.UnitTesting;
using SocialNetwork;
using SocialNetwork.Data.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnitTests
{
   [TestClass]
    public class FriendUnitTests
    {
        [TestMethod]
        public void AddFriend()
        {
            FriendInfo friendInfo = new FriendInfo()
            {
                UserId = 1,
                FriendId = 2,
            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddFriend(friendInfo);

            Assert.AreEqual(1, friendInfo.UserId);
            Assert.AreEqual(2, friendInfo.FriendId);
        }

        [TestMethod]
        public void RemoveFriend()
        {
            FriendInfo friendInfo = new FriendInfo()
            {
                UserId = 1,
                FriendId = 2,
            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddFriend(friendInfo);

            Assert.AreEqual(1, friendInfo.UserId);
            Assert.AreEqual(2, friendInfo.FriendId);

            dataBaseManager.RemoveFriend(friendInfo);
            Assert.AreNotEqual(3, friendInfo.UserId);
            Assert.AreNotEqual(5, friendInfo.FriendId);
        }
    }
}
