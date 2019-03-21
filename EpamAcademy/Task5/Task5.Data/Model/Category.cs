using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace Task5.Data.Model
{
    [Table("Categories")]
    public class Category : Identity<Guid>
    {
        public string Name { get; set; }
    }
}
