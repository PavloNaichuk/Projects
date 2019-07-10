using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;

namespace TicketSalePoint.Data.Models
{
    [Table("Shows")]
    public class Shows
    {
        public Shows() { }
        public int Id { get; set; }
        public int SellerId { get; set; }
        public string Name { get; set; }
        public DateTime Date { get; set; }

        public virtual ICollection<Tickets> Tickets { get; set; }
    }
}
