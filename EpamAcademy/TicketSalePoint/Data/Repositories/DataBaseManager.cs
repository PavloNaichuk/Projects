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

    }