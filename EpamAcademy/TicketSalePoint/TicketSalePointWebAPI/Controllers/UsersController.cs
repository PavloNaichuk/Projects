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
                var query = from user in dataBaseContext.Users
                            select new { user.Id, user.Login, user.FirstName, user.LastName };
                return Ok(await query.ToListAsync());
            }
        }
    }
}
