using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePointData.Context;
using TicketSalePoint.Models;


namespace TicketSalePoint.Repositories
{
    public interface ITicketRepository
    {
        Task Add(Ticket ticket);
        Task Remove(Ticket ticket);
        Task<Ticket> Query(int id);
        Task<IEnumerable<Ticket>> GetAvailableTickets(int showId);
    }
}
