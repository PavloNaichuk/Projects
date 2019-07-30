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
                var query = from seller in dataBaseContext.Theatres
                            select seller.Name;

                return Ok(await query.ToListAsync());
            }
        }
    }
}
