using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using TicketSalePoint.Models;
using TicketSalePoint.Repositories;

namespace TicketSalePoint.Controllers
{
    [Route("api/users")]
    [ApiController]
    public class UserController : Controller
    {
        private UserManager<ApplicationUser> userManager;
        private SignInManager<ApplicationUser> signInManager;

        private readonly IUserRepository userRepository;
    
        public UserController(UserManager<ApplicationUser> userManager, SignInManager<ApplicationUser> signInManager, IUserRepository userRepository)
        {
            this.userManager = userManager;
            this.signInManager = signInManager;
            this.userRepository = userRepository;
        }
       
        [HttpPost]
        [Route("register")]
        public async Task<Object> PostApplicationUser(User user)
        {
            var applicationUser = new ApplicationUser()
            {
                Login = user.Login,
                Email = user.Email,
                FirstName = user.FirstName,
                LastName = user.LastName
            };
            try
            {
                var result = await userManager.CreateAsync(applicationUser, user.Password);
                return Ok(result);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
    }
}
