using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Repositories;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace TicketSalePoint.Controllers
{
    [Route("api/ticket")]
    public class TicketController : Controller
    {
        private readonly TicketRepository ticketRepository;

        public TicketController(TicketRepository ticketRepository)
        {
            this.ticketRepository = ticketRepository;
        }

        public async Task<IActionResult> GetAvailableTickets(int showId)
        {
            return Ok(await this.ticketRepository.GetAvailableTickets(showId));
        }
    }
        
}
