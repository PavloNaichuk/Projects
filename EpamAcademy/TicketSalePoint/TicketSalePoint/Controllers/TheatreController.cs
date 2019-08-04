using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Repositories;

namespace TicketSalePoint.Controllers
{
    [Route("api/theatre")]
    [ApiController]
    public class TheatreController : Controller
    {
        private readonly ITheatreRepository theatreRepository;

        public TheatreController(ITheatreRepository theatreRepository)
        {
            this.theatreRepository = theatreRepository;
        }

        public async Task<IActionResult> GetNames()
        {
            return Ok(await this.theatreRepository.GetNames());
        }
    }

}
