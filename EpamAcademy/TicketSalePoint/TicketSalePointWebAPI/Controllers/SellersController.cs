using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Web.Http;
using TicketSalePoint.Data.Context;
using TicketSalePoint.Data.Models;
using System.Data.Entity;
using System.Threading;
using System.Threading.Tasks;
using System.Web.Http.Description;

namespace TicketSalePointWebAPI.Controllers
{
    public class SellersController : ApiController
    {
        [HttpGet]
        public async Task<IHttpActionResult> GetNames(string name)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from seller in dataBaseContext.Sellers
                            where
                            seller.Name == name
                            select seller;
                return Ok(await query.ToListAsync());
            }
        }
    }
}
