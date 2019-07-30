using System;
using System.Linq;
using System.Web.Http;
using TicketSalePoint.Data.Context;
using System.Data.Entity;
using System.Threading.Tasks;

namespace TicketSalePointWebAPI.Controllers
{
    public class TicketController : ApiController
    {
        [HttpGet]
        public async Task<IHttpActionResult> GetAvailableTickets(int showId)
        {
            using (var dataBaseContext = new DataBaseContext())
            {

                var query = from ticket in dataBaseContext.Tickets
                            where
                                ticket.ShowId == showId
                            select new { ticket.Id, ticket.Row, ticket.Seat, ticket.PriceInCents, ticket.ShowId };
                return Ok(await query.ToListAsync());
            }
        }
    }
}
