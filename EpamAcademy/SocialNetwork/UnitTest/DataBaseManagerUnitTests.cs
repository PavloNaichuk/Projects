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
            DataBaseManager.Instance.ClearDB();
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
            DataBaseManager.Instance.ClearDB();
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
            DataBaseManager.Instance.ClearDB();
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwyc11h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqsh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo2 = new UserInfo()
            {
                Login = "pavlo",
                Password = "pav132212527",
                FirstName = "Pavlo",
                LastName = "Shteapa",
                Email = "paaqsh.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2011, month: 9, day: 10, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo3 = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322227",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.sht.epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 20, second: 25)
            };

            UserInfo userInfo4 = new UserInfo()
            {
                Login = "paskvwyc1h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 24, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo5 = new UserInfo()
            {
                Login = "paskvwycq1h",
                Password = "pav1527",
                FirstName = "Pasa12ha",
                LastName = "Shteapqa",
                Email = "paaqsh.sht12epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 8, day: 20, hour: 18, minute: 30, second: 25)
            };


            DataBaseManager.Instance.AddUser(userInfo1);
            DataBaseManager.Instance.AddUser(userInfo2);
            DataBaseManager.Instance.AddUser(userInfo3);
            DataBaseManager.Instance.AddUser(userInfo4);
            DataBaseManager.Instance.AddUser(userInfo5);

            DataBaseManager.Instance.AddFriendConnection(userInfo1.Id, userInfo2.Id);
            DataBaseManager.Instance.AddFriendConnection(userInfo1.Id, userInfo3.Id);
            DataBaseManager.Instance.AddFriendConnection(userInfo1.Id, userInfo4.Id);

            DataBaseManager.Instance.AddFriendConnection(userInfo4.Id, userInfo5.Id);
            DataBaseManager.Instance.AddFriendConnection(userInfo4.Id, userInfo2.Id);
            DataBaseManager.Instance.AddFriendConnection(userInfo4.Id, userInfo3.Id);

            var friendIds1 = DataBaseManager.Instance.QueryAllFriendIds(userInfo1.Id);
            Assert.AreEqual(3, friendIds1.Count);
            Assert.AreEqual(friendIds1[0].FriendId, userInfo2.Id);
            Assert.AreEqual(friendIds1[1].FriendId, userInfo3.Id);
            Assert.AreEqual(friendIds1[2].FriendId, userInfo4.Id);

            var friendIds2 = DataBaseManager.Instance.QueryAllFriendIds(userInfo2.Id);
            Assert.AreEqual(2, friendIds2.Count);
            Assert.AreEqual(friendIds2[0].FriendId, userInfo1.Id);
            Assert.AreEqual(friendIds2[1].FriendId, userInfo4.Id);

            var friendIds3 = DataBaseManager.Instance.QueryAllFriendIds(userInfo3.Id);
            Assert.AreEqual(2, friendIds3.Count);
            Assert.AreEqual(friendIds3[0].FriendId, userInfo1.Id);
            Assert.AreEqual(friendIds3[1].FriendId, userInfo4.Id);
        }

        [TestMethod]
        public void CountUnreadMessages()
        {
            DataBaseManager.Instance.ClearDB();
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwyc11h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqsh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo2 = new UserInfo()
            {
                Login = "pavlo",
                Password = "pav132212527",
                FirstName = "Pavlo",
                LastName = "Shteapa",
                Email = "paaqsh.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2011, month: 9, day: 10, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo3 = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322227",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.sht.epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 20, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);
            DataBaseManager.Instance.AddUser(userInfo2);
            DataBaseManager.Instance.AddUser(userInfo3);

            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo2.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo2.Id,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 30, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo2.Id, userInfo3.Id),
                SenderId = userInfo2.Id,
                ReceiverId = userInfo3.Id,
                Text = "Hello from Berlin",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 30, second: 25),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo3.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo3.Id,
                Text = "How do you do?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 30, second: 30),
                Unread = true
            };

            MessageInfo messageInfo4 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo3.Id, userInfo2.Id),
                SenderId = userInfo3.Id,
                ReceiverId = userInfo2.Id,
                Text = "Thank you. I am good",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 12, minute: 40, second: 30),
                Unread = true
            };

            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.AddMessage(messageInfo2);
            DataBaseManager.Instance.AddMessage(messageInfo3);
            DataBaseManager.Instance.AddMessage(messageInfo4);

            var messageList1 = DataBaseManager.Instance.CountUnreadMessages(userInfo2.Id);
            Assert.AreEqual(2, messageList1);

            DataBaseManager.Instance.MarkMessageAsRead(messageInfo1.Id);

            var messageList2 = DataBaseManager.Instance.CountUnreadMessages(userInfo2.Id);
            Assert.AreEqual(1, messageList2);
        }

        [TestMethod]
        public void QueryMessagesFromConversation()
        {
            DataBaseManager.Instance.ClearDB();
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwyc11h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqsh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo2 = new UserInfo()
            {
                Login = "pavlo",
                Password = "pav132212527",
                FirstName = "Pavlo",
                LastName = "Shteapa",
                Email = "paaqsh.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2011, month: 9, day: 10, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo3 = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322227",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.sht.epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 20, second: 25)
            };

            UserInfo userInfo4 = new UserInfo()
            {
                Login = "paskvwyc1h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 24, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);
            DataBaseManager.Instance.AddUser(userInfo2);
            DataBaseManager.Instance.AddUser(userInfo3);
            DataBaseManager.Instance.AddUser(userInfo4);

            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo2.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo2.Id,
                Text = "Hello from Rome",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 10, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo2.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo2.Id,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 12, second: 21),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo2.Id, userInfo1.Id),
                SenderId = userInfo2.Id,
                ReceiverId = userInfo1.Id,
                Text = "Hello. I am fine",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 22, second: 23),
                Unread = true
            };

            MessageInfo messageInfo4 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo3.Id, userInfo4.Id),
                SenderId = userInfo3.Id,
                ReceiverId = userInfo4.Id,
                Text = "Hi",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 34, second: 23),
                Unread = true
            };

            MessageInfo messageInfo5 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo4.Id, userInfo3.Id),
                SenderId = userInfo4.Id,
                ReceiverId = userInfo3.Id,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 34, second: 23),
                Unread = true
            };

            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.AddMessage(messageInfo2);
            DataBaseManager.Instance.AddMessage(messageInfo3);
            DataBaseManager.Instance.AddMessage(messageInfo4);
            DataBaseManager.Instance.AddMessage(messageInfo5);

            var messageList1 = DataBaseManager.Instance.QueryMessagesFromConversation(userInfo1.Id, userInfo2.Id);
            Assert.AreEqual(3, messageList1.Count);

            VerifyMessages(messageList1[0], messageInfo1);
            VerifyMessages(messageList1[1], messageInfo2);
            VerifyMessages(messageList1[2], messageInfo3);

            var messageList2 = DataBaseManager.Instance.QueryMessagesFromConversation(userInfo3.Id, userInfo4.Id);
            VerifyMessages(messageList2[0], messageInfo4);
            VerifyMessages(messageList2[1], messageInfo5);
        }

        [TestMethod]
        public void QueryLastMessageFromEachConversation()
        {
            DataBaseManager.Instance.ClearDB();
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwyc11h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqsh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo2 = new UserInfo()
            {
                Login = "pavlo",
                Password = "pav132212527",
                FirstName = "Pavlo",
                LastName = "Shteapa",
                Email = "paaqsh.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2011, month: 9, day: 10, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo3 = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322227",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.sht.epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 20, second: 25)
            };

            UserInfo userInfo4 = new UserInfo()
            {
                Login = "paskvwyc1h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 24, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);
            DataBaseManager.Instance.AddUser(userInfo2);
            DataBaseManager.Instance.AddUser(userInfo3);
            DataBaseManager.Instance.AddUser(userInfo4);

            MessageInfo messageInfo1 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo2.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo2.Id,
                Text = "Hello from New York",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 20, second: 25),
                Unread = true
            };

            MessageInfo messageInfo2 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo2.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo2.Id,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 25, second: 21),
                Unread = true
            };

            MessageInfo messageInfo3 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo2.Id, userInfo1.Id),
                SenderId = userInfo2.Id,
                ReceiverId = userInfo1.Id,
                Text = "Hello. I am fine",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 30, second: 23),
                Unread = true
            };

            MessageInfo messageInfo4 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo3.Id, userInfo1.Id),
                SenderId = userInfo3.Id,
                ReceiverId = userInfo1.Id,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 44, second: 23),
                Unread = true
            };

            MessageInfo messageInfo5 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo3.Id, userInfo1.Id),
                SenderId = userInfo3.Id,
                ReceiverId = userInfo1.Id,
                Text = "Hi",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 47, second: 23),
                Unread = true
            };
            

            MessageInfo messageInfo6 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo4.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo4.Id,
                Text = "Hello",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 44, second: 23),
                Unread = true
            };

            MessageInfo messageInfo7 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo4.Id, userInfo1.Id),
                SenderId = userInfo4.Id,
                ReceiverId = userInfo1.Id,
                Text = "Hello Vova",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 49, second: 23),
                Unread = true
            };

            MessageInfo messageInfo8 = new MessageInfo()
            {
                ConversationId = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo4.Id),
                SenderId = userInfo1.Id,
                ReceiverId = userInfo4.Id,
                Text = "How are you?",
                Date = new DateTime(year: 2015, month: 2, day: 4, hour: 22, minute: 53, second: 23),
                Unread = true
            };

            DataBaseManager.Instance.AddMessage(messageInfo1);
            DataBaseManager.Instance.AddMessage(messageInfo2);
            DataBaseManager.Instance.AddMessage(messageInfo3);
            DataBaseManager.Instance.AddMessage(messageInfo4);
            DataBaseManager.Instance.AddMessage(messageInfo5);
            DataBaseManager.Instance.AddMessage(messageInfo6);
            DataBaseManager.Instance.AddMessage(messageInfo7);
            DataBaseManager.Instance.AddMessage(messageInfo8);

            var messageList = DataBaseManager.Instance.QueryLastMessageFromEachConversation(userInfo1.Id);
            Assert.AreEqual(3, messageList.Count);
            VerifyMessages(messageList[0], messageInfo3);
            VerifyMessages(messageList[1], messageInfo5);
            VerifyMessages(messageList[2], messageInfo8);
        }


        [TestMethod]
        public void QueryConversationId()
        {
            DataBaseManager.Instance.ClearDB();
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322227",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.sht.epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 20, second: 25)
            };

            UserInfo userInfo2 = new UserInfo()
            {
                Login = "paskvwyc1h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 24, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);
            DataBaseManager.Instance.AddUser(userInfo2);

            int conversationId1 = DataBaseManager.Instance.QueryConversationId(userInfo1.Id, userInfo2.Id);
            int conversationId2 = DataBaseManager.Instance.QueryConversationId(userInfo2.Id, userInfo1.Id);

            Assert.AreEqual(conversationId1, conversationId2);
        }

        [TestMethod]
        public void QueryAllFriends()
        {
            DataBaseManager.Instance.ClearDB();
            UserInfo userInfo1 = new UserInfo()
            {
                Login = "paskvwyc11h",
                Password = "pav13222527",
                FirstName = "Pasa1ha",
                LastName = "Shteap1a",
                Email = "paaqsh.sht1epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo2 = new UserInfo()
            {
                Login = "pavlo",
                Password = "pav132212527",
                FirstName = "Pavlo",
                LastName = "Shteapa",
                Email = "paaqsh.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2011, month: 9, day: 10, hour: 18, minute: 30, second: 25)
            };

            UserInfo userInfo3 = new UserInfo()
            {
                Login = "paskvwych",
                Password = "pav1322227",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "paaqsh.sht.epa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 20, second: 25)
            };

            DataBaseManager.Instance.AddUser(userInfo1);
            DataBaseManager.Instance.AddUser(userInfo2);
            DataBaseManager.Instance.AddUser(userInfo3);

            DataBaseManager.Instance.AddFriendConnection(userInfo1.Id, userInfo2.Id);
            DataBaseManager.Instance.AddFriendConnection(userInfo1.Id, userInfo3.Id);

            var friendsList = DataBaseManager.Instance.QueryAllFriends(userInfo1.Id);
            Assert.AreEqual(2, friendsList.Count);
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
