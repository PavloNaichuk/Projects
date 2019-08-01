using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace TicketSalePoint.Models
{
    public class Show : DbContext
    {
        [Key]
        public int Id { get; set; }

        [Key]
        public int TheatreId { get; set; }

        [Required(ErrorMessage = "Name is required")]
        public string Name { get; set; }

        [Required(ErrorMessage = "Date is required")]
        public DateTime Date { get; set; }

        public virtual ICollection<Ticket> Tickets { get; set; }
        public virtual Theatre Theatre { get; set; }
    }
}
