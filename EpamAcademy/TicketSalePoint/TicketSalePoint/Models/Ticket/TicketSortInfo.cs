using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace TicketSalePoint.Models
{
    public class TicketSortInfo
    {
        public int Id { get; set; }
        public int ShowId { get; set; }
        public int Row { get; set; }
        public int Seat { get; set; }
        public int PriceInCents { get; set; }
    }
}
