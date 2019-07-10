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

        public void AddSeller(Sellers seller)
        {
            dataBaseContext.Sellers.Add(seller);
            dataBaseContext.SaveChanges();
        }

        public void RemoveSeller(Sellers seller)
        {
            dataBaseContext.Sellers.Remove(seller);
            dataBaseContext.SaveChanges();
        }

        public void AddShow(Shows show)
        {
            dataBaseContext.Shows.Add(show);
            dataBaseContext.SaveChanges();
        }

        public void RemoveShow(Shows show)
        {
            dataBaseContext.Shows.Remove(show);
            dataBaseContext.SaveChanges();
        }

        public void AddTicket(Tickets ticket)
        {
            dataBaseContext.Tickets.Add(ticket);
            dataBaseContext.SaveChanges();
        }

        public void RemoveTicket(Tickets ticket)
        {
            dataBaseContext.Tickets.Remove(ticket);
            dataBaseContext.SaveChanges();
        }


    }
}