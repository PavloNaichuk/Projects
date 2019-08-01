using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace TicketSalePoint.Models
{
    //[Table("Users")]
    public class User
    {
        public User() { }
        public int Id { get; set; }
        public string Login { get; set; }
        public string Password { get; set; }
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public string Email { get; set; }
        public DateTime RegistrationDate { get; set; }

        public virtual ICollection<Ticket> Tickets { get; set; }
    }
}
