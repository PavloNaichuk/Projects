using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Context
{
    public class DataBaseContext : DbContext
    {
        public DataBaseContext(DbContextOptions<DataBaseContext> options) : base(options)
        {
        }

        public static void ExecuteCommand(string command)
        {
            throw new NotImplementedException();
        }

        public DbSet<User> Users { get; set; }
        public DbSet<Theatre> Theatres { get; set; }
        public DbSet<Show> Shows { get; set; }
        public DbSet<Ticket> Tickets { get; set; }
    }
}
