using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
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

        public async Task<IEnumerable<UserSortInfo>> GetUser(int id)
        {
            var query = from user in dataBaseContext.Users
                        select new UserSortInfo { Id = user.Id, Login = user.Login, FirstName = user.FirstName, LastName = user.LastName };
            return await query.ToListAsync();
        } 
    }
}
