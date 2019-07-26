using System;
using System.Linq;
using System.Web.Http;
using TicketSalePoint.Data.Context;
using System.Data.Entity;
using System.Threading.Tasks;

namespace TicketSalePointWebAPI.Controllers
{
    public class TicketsController : ApiController
    {
        [HttpGet]
        public async Task<IHttpActionResult> GetTicket(int id)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var ticket = await dataBaseContext.Tickets.FirstOrDefaultAsync((t) => t.Id == id);
                if (ticket == null)
                    return NotFound();
                return Ok(ticket);
            }
        }
    }
}
