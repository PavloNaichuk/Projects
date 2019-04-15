using Microsoft.VisualStudio.TestTools.UnitTesting;
using SocialNetwork;
using SocialNetwork.Data.Context;
using SocialNetwork.Data.Models;
using System;

namespace UnitTests
{
    [TestClass]
    public class DataBaseManagerUnitTests
    {
        [TestMethod]
        public void AddUser()
        {
            DataBaseContext.ExecuteCommand("DELETE FROM SocialNetwork");
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "pashkvych",
                Password = "pav132257",
                FirstName = "PashaNew",
                LastName = "ShtepaNew",
                Email = "paqsh.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);

            UserInfo userInfo2 = DataBaseManager.Instance.QueryUserInfo(userInfo1.Login);
            Assert.IsNotNull(userInfo2);
            Assert.AreEqual(userInfo1.Login, userInfo2.Login);
            Assert.AreEqual(userInfo1.Password, userInfo2.Password);
            Assert.AreEqual(userInfo1.FirstName, userInfo2.FirstName);
            Assert.AreEqual(userInfo1.LastName, userInfo2.LastName);
            Assert.AreEqual(userInfo1.Email, userInfo2.Email);
            Assert.AreEqual(userInfo1.RegistrationDate, userInfo2.RegistrationDate);
        }

        [TestMethod]
        public void RemoveUser()
        {
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwyc11h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqsh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);
            UserInfo userInfo2 = DataBaseManager.Instance.QueryUserInfo(userInfo1.Login);
            Assert.IsNotNull(userInfo2);

            DataBaseManager.Instance.RemoveUser(userInfo1);
            UserInfo userInfo3 = DataBaseManager.Instance.QueryUserInfo(userInfo1.Login);
            Assert.IsNull(userInfo3);
        }

        [TestMethod]
        public void AddFriendConnection()
        {
            int userId1 = 1;
            int userId2 = 2;
            int userId3 = 3;
            int userId4 = 4;
            int userId5 = 5;

            DataBaseManager.Instance.AddFriendConnection(userId1, userId2);
            DataBaseManager.Instance.AddFriendConnection(userId1, userId3);
            DataBaseManager.Instance.AddFriendConnection(userId1, userId4);

            DataBaseManager.Instance.AddFriendConnection(userId4, userId5);
            DataBaseManager.Instance.AddFriendConnection(userId4, userId2);
            DataBaseManager.Instance.AddFriendConnection(userId4, userId3);

            var friendIds1 = DataBaseManager.Instance.QueryAllFriendIds(userId1);
            Assert.AreEqual(3, friendIds1.Count);
            Assert.AreEqual(friendIds1[0].FriendId, userId2);
            Assert.AreEqual(friendIds1[1].FriendId, userId3);
            Assert.AreEqual(friendIds1[2].FriendId, userId4);

            var friendIds2 = DataBaseManager.Instance.QueryAllFriendIds(userId2);
            Assert.AreEqual(1, friendIds2.Count);
            Assert.AreEqual(friendIds2[0].FriendId, userId1);

            var friendIds3 = DataBaseManager.Instance.QueryAllFriendIds(userId3);
            Assert.AreEqual(2, friendIds3.Count);
            Assert.AreEqual(friendIds3[0].FriendId, userId1);
            Assert.AreEqual(friendIds3[1].FriendId, userId4);
        }

        [TestMethod]
        public void RemoveFriendConnection()
        {
            int userId1 = 6;
            int userId2 = 7;
            int userId3 = 8;
            int userId4 = 9;
            int userId5 = 10;

            DataBaseManager.Instance.AddFriendConnection(userId1, userId2);
            DataBaseManager.Instance.AddFriendConnection(userId1, userId3);
            DataBaseManager.Instance.AddFriendConnection(userId1, userId4);
            DataBaseManager.Instance.AddFriendConnection(userId1, userId5);

            DataBaseManager.Instance.AddFriendConnection(userId3, userId4);
            DataBaseManager.Instance.AddFriendConnection(userId3, userId5);

            Assert.AreEqual(4, DataBaseManager.Instance.QueryAllFriendIds(userId1).Count);
            Assert.AreEqual(1, DataBaseManager.Instance.QueryAllFriendIds(userId3).Count);
            Assert.AreEqual(2, DataBaseManager.Instance.QueryAllFriendIds(userId4).Count);
            Assert.AreEqual(2, DataBaseManager.Instance.QueryAllFriendIds(userId5).Count);

            {
                DataBaseManager.Instance.RemoveFriendConnection(userId2, userId1);

                var friendIds2 = DataBaseManager.Instance.QueryAllFriendIds(userId2);
                Assert.AreEqual(0, friendIds2.Count);

                var friendIds1 = DataBaseManager.Instance.QueryAllFriendIds(userId1);
                Assert.AreEqual(3, friendIds1.Count);
                Assert.AreEqual(friendIds1[0].FriendId, userId3);
                Assert.AreEqual(friendIds1[1].FriendId, userId4);
                Assert.AreEqual(friendIds1[2].FriendId, userId5);
            }

            {
                DataBaseManager.Instance.RemoveFriendConnection(userId5, userId3);

                var friendIds3 = DataBaseManager.Instance.QueryAllFriendIds(userId3);
                Assert.AreEqual(2, friendIds3.Count);
                Assert.AreEqual(friendIds3[0].FriendId, userId1);
                Assert.AreEqual(friendIds3[1].FriendId, userId4);

                var friendIds5 = DataBaseManager.Instance.QueryAllFriendIds(userId5);
                Assert.AreEqual(1, friendIds5.Count);
                Assert.AreEqual(friendIds5[0].FriendId, userId1);
            }
        }

        [TestMethod]
        public void CountUnreadMessages()
        {
            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(1, 2),
                SenderId = 1,
                ReceiverId = 2,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 30, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(2, 3),
                SenderId = 2,
                ReceiverId = 3,
                Text = "Hello from Berlin",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 30, second: 25),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(2, 3),
                SenderId = 2,
                ReceiverId = 3,
                Text = "How do you do?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 30, second: 30),
                Unread = true
            };

            MessageInfo messageInfo4 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(3, 2),
                SenderId = 3,
                ReceiverId = 2,
                Text = "Thank you. I am good",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 40, second: 30),
                Unread = true
            };

            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.AddMessage(messageInfo2);
            DataBaseManager.Instance.AddMessage(messageInfo3);
            DataBaseManager.Instance.AddMessage(messageInfo4);

            var messageList1 = DataBaseManager.Instance.CountUnreadMessages(2);
            Assert.AreEqual(4, messageList1);

            DataBaseManager.Instance.MarkMessageAsRead(1);

            var messageList2 = DataBaseManager.Instance.CountUnreadMessages(2);
            Assert.AreEqual(3, messageList2);
        }

        [TestMethod]
        public void QueryMessagesFromConversation()
        {
            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(9, 10),
                SenderId = 9,
                ReceiverId = 10,
                Text = "Hello from Rome",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 10, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(9, 10),
                SenderId = 9,
                ReceiverId = 10,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 12, second: 21),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(10, 9),
                SenderId = 10,
                ReceiverId = 9,
                Text = "Hello. I am fine",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 22, second: 23),
                Unread = true
            };

            MessageInfo messageInfo4 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(11, 12),
                SenderId = 11,
                ReceiverId = 12,
                Text = "Hi",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 34, second: 23),
                Unread = true
            };

            MessageInfo messageInfo5 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(12, 11),
                SenderId = 12,
                ReceiverId = 11,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 34, second: 23),
                Unread = true
            };

            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.AddMessage(messageInfo2);
            DataBaseManager.Instance.AddMessage(messageInfo3);
            DataBaseManager.Instance.AddMessage(messageInfo4);
            DataBaseManager.Instance.AddMessage(messageInfo5);

            var messageList1 = DataBaseManager.Instance.QueryMessagesFromConversation(9, 10);
            Assert.AreEqual(3, messageList1.Count);

            VerifyMessages(messageList1[0], messageInfo1);
            VerifyMessages(messageList1[1], messageInfo2);
            VerifyMessages(messageList1[2], messageInfo3);

            var messageList2 = DataBaseManager.Instance.QueryMessagesFromConversation(11, 12);
            VerifyMessages(messageList2[0], messageInfo4);
            VerifyMessages(messageList2[1], messageInfo5);
        }

        [TestMethod]
        public void QueryLastMessageFromEachConversation()
        {
            /*MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(13, 14),
                SenderId = 13,
                ReceiverId = 14,
                Text = "Hello from New York",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 20, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(13, 14),
                SenderId = 13,
                ReceiverId = 14,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 25, second: 21),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(14, 13),
                SenderId = 14,
                ReceiverId = 13,
                Text = "Hello. I am fine",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 30, second: 23),
                Unread = true
            };

            MessageInfo messageInfo4 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(15, 13),
                SenderId = 15,
                ReceiverId = 13,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 44, second: 23),
                Unread = true
            };

            MessageInfo messageInfo5 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(13, 15),
                SenderId = 13,
                ReceiverId = 15,
                Text = "Hi",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 47, second: 23),
                Unread = true
            };
            

            MessageInfo messageInfo6 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(13, 16),
                SenderId = 13,
                ReceiverId = 16,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 44, second: 23),
                Unread = true
            };

            MessageInfo messageInfo7 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(16, 13),
                SenderId = 16,
                ReceiverId = 13,
                Text = "Hello Vova",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 49, second: 23),
                Unread = true
            };

            MessageInfo messageInfo8 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(13, 16),
                SenderId = 13,
                ReceiverId = 16,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 53, second: 23),
                Unread = true
            };*/

            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(1, 3),
                SenderId = 1,
                ReceiverId = 3,
                Text = "Hello from New York",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 20, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(1, 4),
                SenderId = 1,
                ReceiverId = 4,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 25, second: 21),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(1, 3),
                SenderId = 1,
                ReceiverId = 3,
                Text = "Hello. I am fine",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 30, second: 23),
                Unread = true
            };
            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.AddMessage(messageInfo2);
            DataBaseManager.Instance.AddMessage(messageInfo3);

            var messageList = DataBaseManager.Instance.QueryLastMessageFromEachConversation(1);
            Assert.AreEqual(2, messageList.Count);
            VerifyMessages(messageList[0], messageInfo3);
            //VerifyMessages(messageList[1], messageInfo5);
            //VerifyMessages(messageList[2], messageInfo8);

           // var messageList1 = DataBaseManager.Instance.QueryLastMessageFromEachConversation(100);
        }


        [TestMethod]
        public void QueryConversationId()
        {
            int conversationId1 = DataBaseManager.Instance.QueryConversationId(1, 7);
            int conversationId2 = DataBaseManager.Instance.QueryConversationId(7, 1);

            Assert.AreEqual(conversationId1, conversationId2);
        }

        [TestMethod]
        public void QueryAllFriends()
        {
            int userId1 = 1;
            int userId2 = 2;
            int userId3 = 3;

            DataBaseManager.Instance.AddFriendConnection(userId1, userId2);
            DataBaseManager.Instance.AddFriendConnection(userId1, userId3);

            var friendsList = DataBaseManager.Instance.QueryAllFriends(1);
            Assert.AreEqual(2, friendsList.Count);
            Assert.AreEqual(userId2, friendsList[0]);
            Assert.AreEqual(userId3, friendsList[1]);
        }


        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void InvalidArgument1()
        {
            DataBaseManager.Instance.QueryConversationId(0, 1);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentException), "Expected ArgumentException")]
        public void InvalidArgument2()
        {
            DataBaseManager.Instance.QueryConversationId(1, 0);
        }

        [TestMethod]
        public void MarkMessageAsRead()
        {
            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(1, 2),
                SenderId = 1,
                ReceiverId = 2,
                Text = "Hello fm Canada",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 10, second: 25),
                Unread = true
            };
            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.MarkMessageAsRead(4);
        }

        private void VerifyMessages(MessageInfo messageInfo1, MessageInfo messageInfo2)
        {
            Assert.AreEqual(messageInfo1.Text, messageInfo2.Text);
            Assert.AreEqual(messageInfo1.SenderId, messageInfo2.SenderId);
            Assert.AreEqual(messageInfo1.ReceiverId, messageInfo2.ReceiverId);
            Assert.AreEqual(messageInfo1.Date, messageInfo2.Date);
        }
    }
}
