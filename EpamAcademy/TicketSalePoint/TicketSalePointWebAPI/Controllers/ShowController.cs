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
                            select new { show.Name, show.Id, show.TheatreId, show.Date, TheatreName = show.Theatre.Name};
                            query.OrderByDescending(date => date.Date);
                return Ok(await query.ToListAsync());
            }
        }

        [HttpGet]
        public async Task<IHttpActionResult> GetShowsSortedByDateForTheatre(int theatreId)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                            where
                            show.TheatreId == theatreId
                            select new { show.Name, show.Id, show.Date};
                            query.OrderByDescending(date => date.Date);
                return Ok(await query.ToListAsync());
            }
        }
    }
}
