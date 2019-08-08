using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace TicketSalePoint.Models
{
    public class User
    {
        [Key]
        public int Id { get; set; }

        [Required(ErrorMessage = "Login is required")]
        public string Login { get; set; }

        [Required(ErrorMessage = "PasswordHash is required")]
        public byte[] PasswordHash { get; set; }

        [Required(ErrorMessage = "PasswordSalt is required")]
        public byte[] PasswordSalt { get; set; }

        [Required(ErrorMessage = "First name is required")]
        public string FirstName { get; set; }

        [Required(ErrorMessage = "Last name is required")]
        public string LastName { get; set; }

        [Required(ErrorMessage = "Email is required")]
        public string Email { get; set; }

        [Required(ErrorMessage = "Registration date is required")]
        public DateTime RegistrationDate { get; set; }

        public virtual ICollection<Ticket> Tickets { get; set; }
    }
}
