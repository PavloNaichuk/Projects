using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Task5.Data.Model
{
    [Table("Categories")]
    public class Category : Identity<Guid>
    {
        public string Name { get; set; }
    }
}
