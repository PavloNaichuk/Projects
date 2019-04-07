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
        public void temp()
        {
            DateTime date = new DateTime(year: 2015, month: 7, day: 20, hour: 18, minute: 30, second: 25);
            UserInfo userInfo = new UserInfo()
            {
                Id = 0,
                Login = "pappp",
                Password = "123",
                FirstName = "Pavlik",
                LastName = "Nach",
                Email = "pas@gmail.com",
                RegistrationDate = date
            };
            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddUser(userInfo);
            dataBaseManager.Print();
            Console.WriteLine("-------");
            dataBaseManager.RemoveUser(userInfo);
            dataBaseManager.Print();

        }

    }

}