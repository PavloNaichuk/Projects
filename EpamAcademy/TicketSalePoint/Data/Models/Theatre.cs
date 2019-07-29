using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;

namespace TicketSalePoint.Data.Models
{
    [Table("Theatres")]
    public class Theatre
    {
        public Theatre() { }
        public int Id { get; set; }
        public string Login { get; set; }
        public string Password { get; set; }
        public string Name { get; set; }
        public string Address { get; set; }
        public string Email { get; set; }
        public DateTime RegistrationDate { get; set; }

        public virtual ICollection<Show> Shows { get; set; }
    }
}
