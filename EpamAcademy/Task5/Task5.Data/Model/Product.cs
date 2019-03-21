using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace Task5.Data.Model
{
    [Table("Products")]
    public class Product : Identity<Guid>
    {
        public string Name { get; set; }
        public int Price { get; set; }
        public Guid CategoryId { get; set; }
        public Guid VendorId { get; set; }

        [ForeignKey("CategoryId")]
        public virtual Category Category { get; set; }

        [ForeignKey("VendorId")]
        public virtual Vendor Vendor { get; set; }
    }
}
