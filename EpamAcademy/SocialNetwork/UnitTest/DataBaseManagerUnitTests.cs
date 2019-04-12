using Microsoft.VisualStudio.TestTools.UnitTesting;
using SocialNetwork;
using SocialNetwork.Data.Models;
using System;

namespace UnitTests
{
    [TestClass]
    public class DataBaseManagerUnitTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException), "Expected ArgumentNullException")]
        public void AddUser_NullUserInfoField()
        {
            UserInfo userInfo = new UserInfo();
            userInfo.LastName = null;
            userInfo.Email = null;

            DataBaseManager.Instance.AddUser(userInfo);
        }

        [TestMethod]
        public void AddUser_Success()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            UserInfo userInfo = new UserInfo()
            {
                Login = "pashkvych",
                Password = "pav132257",
                FirstName = "Pasha",
                LastName = "Shtepa",
                Email = "paqsh.shtepa@gmail.com",
                RegistrationDate = date
            };

            DataBaseManager.Instance.AddUser(userInfo);
        }

        [TestMethod]
        public void RemoveUser()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            UserInfo userInfo = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322527",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.shtepa@gmail.com",
                RegistrationDate = date
            };

            DataBaseManager.Instance.AddUser(userInfo);
            DataBaseManager.Instance.RemoveUser(userInfo);
        }

        [TestMethod]
        public void AddFriendConnection()
        {
            FriendInfo friendInfo = new FriendInfo()
            {
                UserId = 1,
                FriendId = 7,
            };
            DataBaseManager.Instance.AddFriendConection(friendInfo.UserId, friendInfo.FriendId);
        }

        [TestMethod]
        public void RemoveFriendConnection()
        {
            FriendInfo friendInfo = new FriendInfo()
            {
                UserId = 8,
                FriendId = 7,
            };
            DataBaseManager.Instance.AddFriendConection(friendInfo.UserId, friendInfo.FriendId);
            DataBaseManager.Instance.RemoveFriendConection(friendInfo.UserId, friendInfo.FriendId);
        }

        [TestMethod]
        public void AddMessage()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            MessageInfo messageInfo = new MessageInfo()
            {
                ConversationId = 1,
                SenderId = 1,
                ReceiverId = 7,
                Text = "Pavlik Naichuk helqqlo",
                Date = date,
                Unread = true
            };
            DataBaseManager.Instance.AddMessage(messageInfo);
        }

        [TestMethod]
        public void CountUnreadMessages()
        {
            var countUnread = DataBaseManager.Instance.CountUnreadMessages(7);
            Assert.AreEqual(1, countUnread);
        }

        [TestMethod]
        public void QueryMessagesFromConversation()
        {
            var messageList = DataBaseManager.Instance.QueryMessagesFromConversation(7, 1);
            Assert.AreEqual(0, messageList.Count);
        }

        [TestMethod]
        public void QueryLastMessageFromEachConversation()
        {
            var messageList = DataBaseManager.Instance.QueryLastMessageFromEachConversation(7);
        }

        [TestMethod]
        public void QueryConversationId()
        {
            int conversationId1 = DataBaseManager.Instance.QueryConversationId(1, 7);
            int conversationId2 = DataBaseManager.Instance.QueryConversationId(7, 1);

            Assert.AreEqual(conversationId1, conversationId2);
        }

        
        /*[TestMethod]
         [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
         public void InvalidArgument1()
         {
             DataBaseManager.Instance.QueryConversationId(0, 1);
         }*/
    }
}
