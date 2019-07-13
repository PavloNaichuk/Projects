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

        public Users QueryUser(string login)
        {
            return dataBaseContext.Users.FirstOrDefault(item => item.Login == login);
        }

        public Sellers QuerySeller(string login)
        {
            return dataBaseContext.Sellers.FirstOrDefault(item => item.Login == login);
        }

        public Shows QueryShow(string name)
        {
            return dataBaseContext.Shows.FirstOrDefault(item => item.Name == name);
        }

        public Tickets QueryTicket(int rowNumber)
        {
            return dataBaseContext.Tickets.FirstOrDefault(item => item.RowNumber == rowNumber);
        }

        public void ClearDB()
        {
           //var tickets = from c in dataBaseContext.Tickets select c;
           //dataBaseContext.Tickets.RemoveRange(tickets);

            var users = from c in dataBaseContext.Users select c;
            dataBaseContext.Users.RemoveRange(users);

            var sellers = from c in dataBaseContext.Sellers select c;
            dataBaseContext.Sellers.RemoveRange(sellers);

            //var shows = from c in dataBaseContext.Shows select c;
            //dataBaseContext.Shows.RemoveRange(shows);

            dataBaseContext.SaveChanges();
        }
    }
}