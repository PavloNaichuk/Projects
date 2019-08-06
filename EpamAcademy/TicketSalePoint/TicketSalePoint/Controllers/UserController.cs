using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Models;
using TicketSalePoint.Repositories;

namespace TicketSalePoint.Controllers
{
    [Route("api/users")]
    [ApiController]
    public class UserController : Controller
    {
        private readonly IUserRepository userRepository;
    
        public UserController(IUserRepository userRepository)
        {
            this.userRepository = userRepository;
        }
        [HttpGet()]
        public async Task<IActionResult> GetUser(int id)
        {
            return Ok(await this.userRepository.GetUser(id));
        }

        [HttpPost("register")]
        public async Task PostUser(User user)
        {
            await this.userRepository.Add(user);
        }
    }
}
