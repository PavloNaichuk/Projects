using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public interface IUserRepository
    {
        Task<User> Authenticate(string login, string password);
        Task Add(User user);

        Task<User> AuthenticateExternal(int id);
        Task<User> AddExternal(int id, string name, string email);

    }
}
