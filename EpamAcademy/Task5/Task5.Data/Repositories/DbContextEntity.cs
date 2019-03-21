using System.Data.Entity;
using System.Reflection;
using Task5.Data.Model;

namespace Task5.Data
{
    public class DbContextEntity : DbContext
    {
        public virtual DbSet<Category> Categoryes { get; set; }
        public virtual DbSet<Product> Products { get; set; }
        public virtual DbSet<Vendor> Vendors { get; set; }

        public DbContextEntity() : base("DEFAULT") { }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Configurations.AddFromAssembly(Assembly.GetExecutingAssembly());
        }
    }
}
