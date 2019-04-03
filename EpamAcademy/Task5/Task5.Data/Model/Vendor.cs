using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task5.Data.Model
{
    [Table("Vendors")]
    public class Vendor : Identity<Guid>
    {
        public string Name { get; set; }
        public string Location { get; set; }
    }
}
