using Microsoft.VisualStudio.TestTools.UnitTesting;
using SocialNetwork;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnitTests
{
    [TestClass]
    public class SocialDataServiceTest
    {
        [TestMethod]
        public void CheckIfUnreadMessagesArePresent()
        {
            var dataBaseManager = new DataBaseManager();
            var hasUnread = dataBaseManager.CheckIfHasUnreadMessages(1);
            Assert.IsTrue(hasUnread);
        }

        [TestMethod]
        public void FetchMessagesFromConversation()
        {
            var dataBaseManager = new DataBaseManager();
            var messageList = dataBaseManager.FetchMessagesFromConversation(1, 2);
            Assert.IsTrue(messageList.Count == 2);
        }

        [TestMethod]
        public void FetchLastMessageFromEachConversation()
        {
            var dataBaseManager = new DataBaseManager();
            var messageList = dataBaseManager.FetchLastMessageFromEachConversation(1);
            Assert.IsTrue(messageList.Count == 2);
        }

    }
}
