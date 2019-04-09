using SocialNetwork;
using SocialNetwork.Data.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Data.Repositories
{
    public class FriendRepository
    {
        public void temp()
        {
            FriendInfo friendInfo = new FriendInfo()
            {
                UserId = 2,
                FriendId = 5,

            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddFriend(friendInfo);
            dataBaseManager.PrintUserInfo();
            Console.WriteLine("-------");
            //dataBaseManager.RemoveFriend(friendInfo);
            //dataBaseManager.PrintUserInfo();
        }

    }

}
