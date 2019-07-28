using TicketSalePoint.Data.Context;
using TicketSalePoint.Data.Models;
using System.Linq;

namespace TicketSalePoint
{
    public sealed class DataBaseManager
    {
        private DataBaseManager()
        {
        }

        private DataBaseContext dataBaseContext = new DataBaseContext();

        public static readonly DataBaseManager Instance = new DataBaseManager();

        public void AddUser(User user)
        {
            dataBaseContext.Users.Add(user);
            dataBaseContext.SaveChanges();
        }

        public void RemoveUser(User user)
        {
            dataBaseContext.Users.Remove(user);
            dataBaseContext.SaveChanges();
        }

        public void AddSeller(Theatre theatre)
        {
            dataBaseContext.Theatres.Add(theatre);
            dataBaseContext.SaveChanges();
        }

        public void RemoveSeller(Theatre theatre)
        {
            dataBaseContext.Theatres.Remove(theatre);
            dataBaseContext.SaveChanges();
        }

        public void AddShow(Show show)
        {
            dataBaseContext.Shows.Add(show);
            dataBaseContext.SaveChanges();
        }

        public void RemoveShow(Show show)
        {
            dataBaseContext.Shows.Remove(show);
            dataBaseContext.SaveChanges();
        }

        public void AddTicket(Ticket ticket)
        {
            dataBaseContext.Tickets.Add(ticket);
            dataBaseContext.SaveChanges();
        }

        public void RemoveTicket(Ticket ticket)
        {
            dataBaseContext.Tickets.Remove(ticket);
            dataBaseContext.SaveChanges();
        }

        public User QueryUser(int id)
        {
            return dataBaseContext.Users.FirstOrDefault(item => item.Id == id);
        }

        public Theatre QueryTheatre(int id)
        {
            return dataBaseContext.Theatres.FirstOrDefault(item => item.Id == id);
        }

        public Show QueryShow(int id)
        {
            return dataBaseContext.Shows.FirstOrDefault(item => item.Id == id);
        }

        public Ticket QueryTicket(int id)
        {
            return dataBaseContext.Tickets.FirstOrDefault(item => item.Id == id);
        }

        public void ClearDB()
        {
            var users = from c in dataBaseContext.Users select c;
            dataBaseContext.Users.RemoveRange(users);

            dataBaseContext.SaveChanges();
        }
    }
}