using System.Data.Entity;
using SocialNetwork.Data.Models;

namespace SocialNetwork.Data.Context
{
    public class DataBaseContext : DbContext
    {
        public DataBaseContext() : base("DbConnection")
        { }

        public DbSet<UserInfo> UserInfo { get; set; }
    }
}
