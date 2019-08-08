using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Repositories;

namespace TicketSalePointData.Controllers
{
    [Route("api/shows")]
    [ApiController]
    public class ShowController : Controller
    {
        private readonly IShowRepository showRepository;

        public ShowController(IShowRepository showRepository)
        {
            this.showRepository = showRepository;
        }

        [HttpGet()]
        public async Task<IActionResult> GetShowsSortedByDateForAllTheatres()
        {
            return Ok(await this.showRepository.GetShowsSortedByDateForAllTheatres());
        }

        [HttpGet("Get/{theatreId}")]
        public async Task<IActionResult> GetShowsSortedByDateForTheatre(int theatreId)
        {
            return Ok(await this.showRepository.GetShowsSortedByDateForTheatre(theatreId));
        }
    }
      
}
