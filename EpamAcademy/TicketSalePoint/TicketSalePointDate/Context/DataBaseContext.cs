using Microsoft.EntityFrameworkCore;
using TicketSalePointData.Models;

namespace TicketSalePointData.Context
{
    public class DataBaseContext : DbContext
    {
        public DataBaseContext()
        {
        }

        public DataBaseContext(DbContextOptions options) : base(options)
        {
        }

        public DbSet<User> Users { get; set; }
        public DbSet<Theatre> Theatres { get; set; }
        public DbSet<Show> Shows { get; set; }
        public DbSet<Ticket> Tickets { get; set; }
    }
}
