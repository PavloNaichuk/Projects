using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public class TicketRepository : ITicketRepository
    {
        private readonly DataBaseContext dataBaseContext;

        public TicketRepository(DataBaseContext context)
        {
            this.dataBaseContext = context;
        }

        public async Task Add(Ticket ticket)
        {
            dataBaseContext.Tickets.Add(ticket);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task Remove(Ticket ticket)
        {
            dataBaseContext.Tickets.Remove(ticket);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task<Ticket> Query(int id)
        {
            return await dataBaseContext.Tickets.FirstOrDefaultAsync(item => item.Id == id);
        }

        public async Task<IEnumerable<Ticket>> GetAvailableTickets(int showId)
        {
            var query = from ticket in dataBaseContext.Tickets
                        where
                            ticket.ShowId == showId
                        select new Ticket { Id = ticket.Id, Row = ticket.Row, Seat = ticket.Seat, PriceInCents = ticket.PriceInCents, ShowId = ticket.ShowId };
            return await query.ToListAsync();
        }
    }
}
