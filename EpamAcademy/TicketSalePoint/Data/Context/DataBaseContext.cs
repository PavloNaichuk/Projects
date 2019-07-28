using System;
using System.Data.Entity;
using TicketSalePoint.Data.Models;

namespace TicketSalePoint.Data.Context
{
    public class DataBaseContext : DbContext
    {
        public DataBaseContext() : base("DbConnection")
        {
           Database.SetInitializer<DataBaseContext>(null);
        }

        public static void ExecuteCommand(string command)
        {
            throw new NotImplementedException();
        }

        public DbSet<User> Users { get; set; }
        public DbSet<Theatre> Theatres{ get; set; }
        public DbSet<Show> Shows { get; set; }
        public DbSet<Ticket> Tickets { get; set; }
    }
}
