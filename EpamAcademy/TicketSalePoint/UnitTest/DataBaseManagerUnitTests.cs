using Microsoft.VisualStudio.TestTools.UnitTesting;
using TicketSalePoint;
using TicketSalePoint.Data.Context;
using TicketSalePoint.Data.Models;
using System;

namespace UnitTest
{
    [TestClass]
    public class DataBaseManagerUnitTests
    {
        [TestMethod]
        public void AddUser()
        {
            DataBaseManager.Instance.ClearDB();
            Users user1 = new Users()
            {
                Login = "pashkevych",
                Password = "pav13222527",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "pasha@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(user1);

            Users user2 = DataBaseManager.Instance.QueryUser(user1.Login);
            Assert.IsNotNull(user2);
            Assert.AreEqual(user1.Login, user2.Login);
        }

        [TestMethod]
        public void RemoveUser()
        {
            DataBaseManager.Instance.ClearDB();
            Users user1 = new Users()
            {
                Login = "pasha.naichuk",
                Password = "pav13222527",
                FirstName = "Pasaha",
                LastName = "Shteapa",
                Email = "pasha@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddUser(user1);
            Users user2 = DataBaseManager.Instance.QueryUser(user1.Login);
            Assert.IsNotNull(user2);

            DataBaseManager.Instance.RemoveUser(user1);
            Users userInfo3 = DataBaseManager.Instance.QueryUser(user1.Login);
            Assert.IsNull(userInfo3);
        }
        [TestMethod]
        public void AddSeller()
        {
            //DataBaseManager.Instance.ClearDB();
            Sellers seller1 = new Sellers()
            {
                Login = "Dimon",
                Password = "propan1233",
                Name = "Pasaha",
                Address = "Kyidsssdsv",
                Email = "dima.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddSeller(seller1);

            Sellers seller2 = DataBaseManager.Instance.QuerySeller(seller1.Login);
            Assert.IsNotNull(seller2);
            Assert.AreEqual(seller1.Login, seller2.Login);
        }

        [TestMethod]
        public void RemoveSeller()
        {
            DataBaseManager.Instance.ClearDB();
            Sellers seller1 = new Sellers()
            {
                Login = "Dimon",
                Password = "propan1",
                Name = "Dima",
                Address = "Kyiv",
                Email = "dima.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)

            };
            DataBaseManager.Instance.AddSeller(seller1);
            Sellers seller2 = DataBaseManager.Instance.QuerySeller(seller1.Login);
            Assert.IsNotNull(seller2);

            DataBaseManager.Instance.RemoveSeller(seller1);
            Sellers seller3 = DataBaseManager.Instance.QuerySeller(seller1.Login);
            Assert.IsNull(seller3);
        }

        /*[TestMethod]
        public void AddShows()
        {
            DataBaseManager.Instance.ClearDB();
            Sellers seller = new Sellers()
            {
                Login = "Dimon",
                Password = "propan1233",
                Name = "Dima",
                Address = "Kyiv",
                Email = "dima.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)

            };
            DataBaseManager.Instance.AddSeller(seller);

            Shows show1 = new Shows()
            {
                SellerId = 1,
                Name = "Mem",
                Date = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };

            DataBaseManager.Instance.AddShow(show1);

            Shows show2 = DataBaseManager.Instance.QueryShow(show1.Name);
            Assert.IsNotNull(show2);
            Assert.AreEqual(show1.Name, show2.Name);
        }

        [TestMethod]
        public void RemoveShow()
        {
            DataBaseManager.Instance.ClearDB();
            Sellers seller = new Sellers()
            {
                Login = "Dimon",
                Password = "propan1233",
                Name = "Dima",
                Address = "Kyiv",
                Email = "dima.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)

            };
            DataBaseManager.Instance.AddSeller(seller);

            Shows show1 = new Shows()
            {
                SellerId = 1,
                Name = "Mem",
                Date = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };
            DataBaseManager.Instance.AddShow(show1);
            Shows show2 = DataBaseManager.Instance.QueryShow(show1.Name);
            Assert.IsNotNull(show2);

            DataBaseManager.Instance.RemoveShow(show1);
            Shows show3 = DataBaseManager.Instance.QueryShow(show1.Name);
            Assert.IsNull(show3);
        }

        [TestMethod]
        public void AddTicket()
        {
            DataBaseManager.Instance.ClearDB();
            Sellers seller = new Sellers()
            {
                Id = 2,
                Login = "Dimon",
                Password = "propan1233",
                Name = "Dima",
                Address = "Kyiv",
                Email = "dima.shtepa@gmail.com",
                RegistrationDate = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)

            };
            DataBaseManager.Instance.AddSeller(seller);

            Shows show = new Shows()
            {
                SellerId = 2,
                Name = "Mem",
                Date = new DateTime(year: 2015, month: 9, day: 20, hour: 18, minute: 30, second: 25)
            };
            DataBaseManager.Instance.AddShow(show);

            Tickets ticket1 = new Tickets()
            {
                ShowId = 1,
                RowNumber = 1,
                SeatNumber = 2,
                Price = 100,
            };
            DataBaseManager.Instance.AddTicket(ticket1);

            Tickets ticket2 = DataBaseManager.Instance.QueryTicket(ticket1.RowNumber);
            Assert.IsNotNull(ticket2);
            Assert.AreEqual(ticket1.RowNumber, ticket2.RowNumber);
        }*/
    }
}
