using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;

namespace TicketSalePoint.Data.Models
{
    [Table("Shows")]
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
