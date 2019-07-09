using System;
using System.Data.Common;
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

        public static void ExecuteCommand(string v)
        {
            throw new NotImplementedException();
        }

        public DbSet<Users> Users { get; set; }
    }
}
