using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace TicketSalePoint.Data.Models
{
    public enum Status
    {
        SOLD,
        FREE,
        RESERVED
    }

    [Table("Tickets")]
    public class Ticket
    {
        public Ticket() { }
        public int Id { get; set; }
        public int ShowId { get; set; }
        public int Row { get; set; }
        public int Seat { get; set; }
        public int PriceCents { get; set; }
        public Status? Status { get; set; }
        public DateTime ReservationDate { get; set; }

        public virtual Show Show { get; set; }
        public virtual User User { get; set; }
    }
}
