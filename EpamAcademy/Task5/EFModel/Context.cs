using Model;
using Model.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EFModel
{
    public class Context: DbContext
    {
        public Context():
            base("DefaultConnection")
        {

        }

        public DbSet<Product> products { get; set; }
        public DbSet<Category> categories { get; set; }
        
    }
}
