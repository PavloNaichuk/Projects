using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    interface IUserRepository
    {
        Task Add(User user);
        Task Remove(User user);
        Task<User> Query(int id);
        Task<IEnumerable<UserSortInfo>> GetUser(int id);
    }
}
