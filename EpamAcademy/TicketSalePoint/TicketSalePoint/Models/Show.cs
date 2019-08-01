using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace TicketSalePoint.Models
{
   // [Table("Shows")]
    public class Show
    {
        public Show() { }
        public int Id { get; set; }
        public int TheatreId { get; set; }
        public string Name { get; set; }
        public DateTime Date { get; set; }

        public virtual ICollection<Ticket> Tickets { get; set; }
        public virtual Theatre Theatre { get; set; }
    }
}
