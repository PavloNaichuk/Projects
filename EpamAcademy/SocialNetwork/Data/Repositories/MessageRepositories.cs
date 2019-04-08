using SocialNetwork;
using SocialNetwork.Data.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Data.Repositories
{
    public class MessageRepositories
    {
        public void temp()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            MessageInfo messageInfo = new MessageInfo()
            {
                Id = 3,
                SenderId = 2,
                ReceiverId = 4,
                Message = "Pavlik Naichuk hello",
                Date = date,
                WasRead = true
            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.SendMessage(messageInfo);
            dataBaseManager.PrintMessageInfo();
        }

    }
}