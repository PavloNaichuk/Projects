using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public interface IUserRepository
    {
        Task Add(User user);
        Task Remove(User user);
        Task<User> Query(int id);
        Task<IEnumerable<User>> GetUser(int id);
    }
}
