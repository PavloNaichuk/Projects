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
    public class MessageUnitTests
    {
        [TestMethod]
        public void SendMessage()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            MessageInfo messageInfo = new MessageInfo()
            {
                SenderId = 1,
                ReceiverId = 2,
                Message = "Pavlik Naichuk helqqlo",
                Date = date,
                WasRead = true
            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.SendMessage(messageInfo);

            Assert.AreEqual(1, messageInfo.SenderId);
            Assert.AreEqual(2, messageInfo.ReceiverId);
            Assert.AreEqual("Pavlik Naichuk helqqlo", messageInfo.Message);
        }
    }
}
