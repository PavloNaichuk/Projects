using System;
using System.Linq;
using System.Web.Http;
using TicketSalePoint.Data.Context;
using System.Data.Entity;
using System.Threading.Tasks;

namespace TicketSalePointWebAPI.Controllers
{
    public class ShowsController : ApiController
    {
        [HttpGet]
        public async Task<IHttpActionResult> GetShowsSortedByDateForAllTheatres()
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                select show.Name, show.Id, show.TheatreId;

                return Ok(await query.ToListAsync());
            }
        }

        [HttpGet]
        public async Task<IHttpActionResult> GetShowsSortedByDateForTheatre(int theatreId)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                            orderby show.TheatreId
                            select show.Name;
                return Ok(await query.ToListAsync());
            }
        }
    }
}
