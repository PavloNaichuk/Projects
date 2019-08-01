using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Context;
using TicketSalePoint.Models;
using TicketSalePoint.Repositories;

namespace TicketSalePoint.Controllers
{
    [Route("api/theatre")]
    [ApiController]
    public class TheatreController : Controller
    {
        private readonly TheatreRepository theatreRepository;

        public TheatreController(TheatreRepository theatreRepository)
        {
            this.theatreRepository = theatreRepository;
        }

        public async Task<IActionResult> GetNames()
        {
            return Ok(await this.theatreRepository.GetNames());
        }
    }

    class BusinessLogicResultGenerator : TheatreRepository
    {
        /*Task<IActionResult> TheatreResult()
        {
         
        }*/
    }
  }
