using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Repositories;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace TicketSalePoint.Controllers
{
    [Route("api/show")]
    public class ShowController : Controller
    {
        private readonly ShowRepository showRepository;

        public ShowController(ShowRepository showRepository)
        {
            this.showRepository = showRepository;
        }

        public async Task<IActionResult> GetShowsSortedByDateForAllTheatres()
        {
            return Ok(await this.showRepository.GetShowsSortedByDateForAllTheatres());
        }

        public async Task<IActionResult> GetShowsSortedByDateForTheatre(int theatreId)
        {
            return Ok(await this.showRepository.GetShowsSortedByDateForTheatre(theatreId));
        }
    }
      
}
