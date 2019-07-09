using Data.Repositories;
using TicketSalePoint.Data.Context;
using TicketSalePoint.Data.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TicketSalePoint
{
    public sealed class DataBaseManager
    {
        public static readonly DataBaseManager Instance = new DataBaseManager();
        private DataBaseManager() { }

        private DataBaseContext dataBaseContext = new DataBaseContext();

        public void AddUser(Users user)
        {
            dataBaseContext.Users.Add(user);
            dataBaseContext.SaveChanges();
        }

        public void RemoveUser(Users user)
        {
            dataBaseContext.Users.Remove(user);
            dataBaseContext.SaveChanges();
        }
    }
}