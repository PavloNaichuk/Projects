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
        public async Task<IHttpActionResult> GetDateNames(DateTime date)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                            where
                            show.Date == date
                            select show.Name;
                return Ok(await query.ToListAsync());
            }
        }

        [HttpGet]
        public async Task<IHttpActionResult> GetDateFromSellersNames(DateTime date)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                            where
                            show.Date == date
                            orderby show.SellerId
                            select show.Name;
                return Ok(await query.ToListAsync());
            }
        }
    }
}
