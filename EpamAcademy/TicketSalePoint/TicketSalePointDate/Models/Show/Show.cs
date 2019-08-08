using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace TicketSalePoint.Models
{
    public class Show
    {
        [Key]
        public int Id { get; set; }

        [ForeignKey("Theatre")]
        public int TheatreId { get; set; }

        [Required(ErrorMessage = "Name is required")]
        public string Name { get; set; }

        [Required(ErrorMessage = "Date is required")]
        public DateTime Date { get; set; }

        public virtual ICollection<Ticket> Tickets { get; set; }
        public virtual Theatre Theatre { get; set; }
    }
}
