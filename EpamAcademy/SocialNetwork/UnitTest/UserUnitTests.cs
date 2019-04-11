using System;
using System.Data.SqlClient;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SocialNetwork;
using SocialNetwork.Data.Models;

namespace UnitTests
{
    [TestClass]
    public class UserUnitTests
    {
        /* [TestMethod]
         [ExpectedException(typeof(SqlException), "Expected ArgumentNullException")]
         public void UserNullArgument()
         {
             UserInfo userInfo = new UserInfo();
             userInfo.LastName = null;
             userInfo.Email = null;

             DataBaseManager dataBaseManager = new DataBaseManager();
             dataBaseManager.AddUser(userInfo);
         }*/

        [TestMethod]
        public void AddUser()
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

            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddUser(userInfo);

            Assert.AreEqual("pashkvych", userInfo.Login);
            Assert.AreEqual("Pasha", userInfo.FirstName);
            Assert.AreEqual("paqsh.shtepa@gmail.com", userInfo.Email);
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

            DataBaseManager dataBaseManager = new DataBaseManager();
            dataBaseManager.AddUser(userInfo);
     
            Assert.AreEqual("paskvwych", userInfo.Login);
            Assert.AreEqual("Pasaha", userInfo.FirstName);
            Assert.AreEqual("paaqsh.shtepa@gmail.com", userInfo.Email);

            /*dataBaseManager.RemoveUser(userInfo);
            Assert.AreNotEqual("paskvwych", userInfo.Login);
            Assert.AreNotEqual("Pasaha", userInfo.FirstName);
            Assert.AreNotEqual("paaqsh.shtepa@gmail.com", userInfo.Email);*/
        }
    }
}
