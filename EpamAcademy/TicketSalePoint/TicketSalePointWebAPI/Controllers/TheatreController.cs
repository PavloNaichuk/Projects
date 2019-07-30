using System.Linq;
using System.Web.Http;
using TicketSalePoint.Data.Context;
using System.Data.Entity;
using System.Threading.Tasks;

namespace TicketSalePointWebAPI.Controllers
{
    public class TheatreController : ApiController
    {
        [HttpGet]
        public async Task<IHttpActionResult> GetNames()
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from theatre in dataBaseContext.Theatres
                            select new { theatre.Id, theatre.Name };

                return Ok(await query.ToListAsync());
            }
        }
    }
}
