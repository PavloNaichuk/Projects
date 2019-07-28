using System;
using System.Linq;
using System.Web.Http;
using TicketSalePoint.Data.Context;
using System.Data.Entity;
using System.Threading.Tasks;

namespace TicketSalePointWebAPI.Controllers
{
    public class UsersController : ApiController
    {
        [HttpGet]
        public async Task<IHttpActionResult> GetUser(int id)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var user = await dataBaseContext.Users.FirstOrDefaultAsync((t) => t.Id == id);
                if (user == null)
                    return NotFound();
                return Ok(user);
            }
        }
    }
}
