using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Model.Models
{
    public class Product
    {
        public int ID { get; set; }
        public string Name { get; set; }
        public int ProducerId { get; set; }
        public int CategoryId { get; set; }

        public virtual Category Category {get;set;}
    }
}
