using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public class UserRepository : IUserRepository
    {
        private readonly DataBaseContext dataBaseContext;

        public UserRepository(DataBaseContext context)
        {
            this.dataBaseContext = context;
        }

        public async Task Add(User user)
        {
            dataBaseContext.Users.Add(user);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task Remove(User user)
        {
            dataBaseContext.Users.Remove(user);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task<User> Query(int id)
        {
            return await dataBaseContext.Users.FirstOrDefaultAsync(item => item.Id == id);
        }

        public async Task<IEnumerable<User>> GetUser(int id)
        {
            var query = from user in dataBaseContext.Users
                        select new User { Id = user.Id, Login = user.Login, FirstName = user.FirstName, LastName = user.LastName };
            return await query.ToListAsync();
            /*if (user != null)
            {
                var tokenDescriptor = new SecurityTokenDescriptor
                {
                    Subject = new ClaimsIdentity(new Claim[]
                    {
                        new Claim("Id", user.Id.ToString())
                    }),
                    Expires = DateTime.UtcNow.AddMinutes(5),
                    SigningCredentials = new SigningCredentials(new SymmetricSecurityKey(Encoding.UTF8.GetBytes("1234567890123456")), SecurityAlgorithms.HmacSha256Signature)
                };
                var tokenHandler = new JwtSecurityTokenHandler();
                var securityToken = tokenHandler.CreateToken(tokenDescriptor);
                var token = tokenHandler.WriteToken(securityToken);
                await token;
            }*/
        }
    }
}
