using System;

namespace TicketSalePoint.Models
{
    public class ShowSortInfo
    {
        public int Id { get; set; }
        public int TheatreId { get; set; }
        public string Name { get; set; }
        public DateTime Date { get; set; }
        public string TheatreName { get; set; }
    }
}
