using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
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
    
        public UserController(IUserRepository userRepository)
        {
            this.userRepository = userRepository;
        }
      
        [HttpPost("token")]
        public ActionResult GetToken()
        {
            //security key
            string securityKey = "this_is_our_supper_long_security_key_for_token_validation_project_2018_09_07$smesk.in";

            //symmetric security key
            var symmetricSecurityKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(securityKey));

            //signing credentials
            var signingCredentials = new SigningCredentials(symmetricSecurityKey, SecurityAlgorithms.HmacSha256Signature);

            //add claims
            var claims = new List<Claim>();
            claims.Add(new Claim(ClaimTypes.Role, "Administrator"));
            claims.Add(new Claim(ClaimTypes.Role, "Reader"));
            claims.Add(new Claim("Our_Custom_Claim", "Our custom value"));
            claims.Add(new Claim("Id", "110"));

            //create token
            var token = new JwtSecurityToken(
                issuer: "smesk.in",
                audience: "readers",
                expires: DateTime.Now.AddHours(1),
                signingCredentials: signingCredentials, 
                claims: claims
                );
            //return token
            return Ok(new JwtSecurityTokenHandler().WriteToken(token));
        }

        [HttpPost("register")]
        public async Task PostUser(User user)
        {
            await this.userRepository.Add(user);
        }
    }
}
