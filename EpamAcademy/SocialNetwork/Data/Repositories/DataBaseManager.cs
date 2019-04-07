using SocialNetwork.Data.Context;
using SocialNetwork.Data.Models;
using System;

namespace SocialNetwork
{
    public class DataBaseManager
    {
        private DataBaseContext dataBaseContext = new DataBaseContext();

        public void AddUser(UserInfo userInfo)
        {
            dataBaseContext.UserInfo.Add(userInfo);
            dataBaseContext.SaveChanges();
        }

        public void RemoveUser(UserInfo userInfo)
        {
            dataBaseContext.UserInfo.Remove(userInfo);
            dataBaseContext.SaveChanges();
        }
        public void Print()
        {
            var users = dataBaseContext.UserInfo;
            Console.WriteLine("List:");
            foreach (UserInfo userInfo in users)
            {
                Console.WriteLine("{0}. \n{1} \n {2}\n {3} \n {4} \n {5} \n {6}", userInfo.Id, userInfo.Login, userInfo.Password,
                    userInfo.FirstName, userInfo.LastName, userInfo.Email, userInfo.RegistrationDate);
            }
        }
    }
}