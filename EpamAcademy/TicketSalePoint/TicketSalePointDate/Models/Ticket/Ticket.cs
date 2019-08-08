using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.EntityFrameworkCore;

namespace TicketSalePointData.Models
{
    public enum TicketStatus
    {
        SOLD,
        FREE,
        RESERVED
    }

    public class Ticket
    {
        [Key]
        public int Id { get; set; }

        [ForeignKey("Show")]
        public int ShowId { get; set; }

        [Required(ErrorMessage = "Row is required")]
        public int Row { get; set; }

        [Required(ErrorMessage = "Seat is required")]
        public int Seat { get; set; }

        [Required(ErrorMessage = "Price in cents is required")]
        public int PriceInCents { get; set; }

        [Required(ErrorMessage = "Status is required")]
        public TicketStatus? Status { get; set; }

        [Required(ErrorMessage = "Reservation date is required")]
        public DateTime ReservationDate { get; set; }

        public virtual Show Show { get; set; }
        public virtual User User { get; set; }
    }
}
