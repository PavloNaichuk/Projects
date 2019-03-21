using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task5.Data
{
    public class DbInitializer : System.Data.Entity.DropCreateDatabaseIfModelChanges<DbContextEntity>
    {
        protected override void Seed(DbContextEntity context)
        {
            base.Seed(context);
        }
    }
}
