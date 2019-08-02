using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;


namespace TicketSalePoint.Repositories
{
    interface ITicketRepository
    {
        Task Add(Ticket ticket);
        Task Remove(Ticket ticket);
        Task<Ticket> Query(int id);
        Task<IEnumerable<TicketSortInfo>> GetAvailableTickets(int showId);
    }
}
