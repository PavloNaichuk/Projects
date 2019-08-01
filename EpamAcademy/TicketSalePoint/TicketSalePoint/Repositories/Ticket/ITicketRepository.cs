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
        void Add(Ticket ticket);
        void Remove(Ticket ticket);
        Ticket Query(int id);
        Task<IEnumerable<TicketSortInfo>> GetAvailableTickets(int showId);
    }
}
