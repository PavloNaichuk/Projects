using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace Task5.Data.Model
{
    [Table("Vendors")]
    public class Vendor : Identity<Guid>
    {
        public string Name { get; set; }
        public string Location { get; set; }
    }
}
