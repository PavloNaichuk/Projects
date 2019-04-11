using SocialNetwork;
using SocialNetwork.Data.Models;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Data.Repositories
{
    public class UserRepository
    {
        public void ConsoleWrite()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            UserInfo userInfo = new UserInfo()
            {
                Id = 0,
                Login = "ppвpwwwp",
                Password = "12322223",
                FirstName = "Hahaыhah",
                LastName = "Hahыыыahaa",
                Email = "paaaыфas@gmail.com",
                RegistrationDate = date
            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddUser(userInfo);
            dataBaseManager.PrintUserInfo();
            Console.WriteLine("-------");
            //dataBaseManager.RemoveUser(userInfo);
            //dataBaseManager.PrintUserInfo();
        }

    }

}