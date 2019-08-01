using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    interface IUserRepository
    {
        void Add(User user);
        void Remove(User user);
        User Query(int id);
        Task<IEnumerable<UserSortInfo>> GetUser(int id);
    }
}
