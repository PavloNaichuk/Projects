using System.Data.Common;
using System.Data.Entity;
using SocialNetwork.Data.Models;

namespace SocialNetwork.Data.Context
{
    public class DataBaseContext : DbContext
    {
        public DataBaseContext() : base("DbConnection")
        {
           Database.SetInitializer<DataBaseContext>(null);
        }
        public DbSet<UserInfo> UserInfo { get; set; }
        public DbSet<MessageInfo> MessageInfo { get; set; }
        public DbSet<FriendInfo> FriendInfo { get; set; }
    }
}
