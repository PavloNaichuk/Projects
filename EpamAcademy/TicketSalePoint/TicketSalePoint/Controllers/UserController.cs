using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Options;
using Microsoft.IdentityModel.Tokens;
using TicketSalePoint.Models;
using TicketSalePoint.Repositories;

namespace TicketSalePoint.Controllers
{
    [Route("api/users")]
    [ApiController]
    public class UserController : Controller
    {
        private readonly IUserRepository userRepository;
        private readonly ApplicationSettings applicationSettings;
    
        public UserController(IUserRepository userRepository, IOptions<ApplicationSettings> applicationSettings)
        {
            this.userRepository = userRepository;
            this.applicationSettings = applicationSettings.Value;
        }
        [HttpGet()]
        public async Task<IActionResult> GetUser(int id)
        {
            return Ok(await this.userRepository.GetUser(id));
        }

        [HttpPost("register")]
        public async Task<IActionResult> PostUser(User user)
        {
            return Ok(this.userRepository.Add(user));
        }
    }
}
