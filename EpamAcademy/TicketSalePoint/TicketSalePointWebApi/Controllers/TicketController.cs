using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Repositories;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace TicketSalePointData.Controllers
{
    [Route("api/ticket")]
    [ApiController]
    public class TicketController : Controller
    {
        private readonly ITicketRepository ticketRepository;

        public TicketController(ITicketRepository ticketRepository)
        {
            this.ticketRepository = ticketRepository;
        }

        public async Task<IActionResult> GetAvailableTickets(int showId)
        {
            return Ok(await this.ticketRepository.GetAvailableTickets(showId));
        }
    }
        
}
