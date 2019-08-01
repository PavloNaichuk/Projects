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
        public void Add(Ticket ticket)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Tickets.Add(ticket);
                dataBaseContext.SaveChanges();
            }
        }

        public void Remove(Ticket ticket)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Tickets.Remove(ticket);
                dataBaseContext.SaveChanges();
            }
        }

        public Ticket Query(int id)
        {
            using (var dataBaseContext = new DataBaseContext())
                return dataBaseContext.Tickets.FirstOrDefault(item => item.Id == id);
        }

        public async Task<IEnumerable<TicketSortInfo>> GetAvailableTickets(int showId)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from ticket in dataBaseContext.Tickets
                            where
                                ticket.ShowId == showId
                            select new TicketSortInfo { Id = ticket.Id, Row = ticket.Row, Seat = ticket.Seat, PriceInCents = ticket.PriceInCents, ShowId = ticket.ShowId };
                return await query.ToListAsync();
            }
        }
    }
}
