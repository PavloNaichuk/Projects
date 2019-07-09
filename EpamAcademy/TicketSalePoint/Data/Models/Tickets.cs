using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;

namespace TicketSalePoint.Data.Models
{
    [Table("Tickets")]
    public class Tickets
    {
        public Tickets() { }
        public int Id { get; set; }
        public int ShowId { get; set; }
        public int RowNumber { get; set; }
        public int SeatNumber { get; set; }
        public int Price { get; set; }
        public int Status { get; set; }
    }
}
