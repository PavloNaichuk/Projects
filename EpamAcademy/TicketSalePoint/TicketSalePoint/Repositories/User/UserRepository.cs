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
        public void Add(User user)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Users.Add(user);
                dataBaseContext.SaveChanges();
            }
        }

        public void Remove(User user)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Users.Remove(user);
                dataBaseContext.SaveChanges();
            }
        }

        public User Query(int id)
        {
            using (var dataBaseContext = new DataBaseContext())
                return dataBaseContext.Users.FirstOrDefault(item => item.Id == id);
        }

        public async Task<IEnumerable<UserSortInfo>> GetUser(int id)
        {
            using(var dataBaseContext = new DataBaseContext())
            {
                var query = from user in dataBaseContext.Users
                            select new  UserSortInfo{ Id = user.Id, Login =  user.Login, FirstName = user.FirstName, LastName = user.LastName };
                return await query.ToListAsync();
            }
        } 
    }
}
