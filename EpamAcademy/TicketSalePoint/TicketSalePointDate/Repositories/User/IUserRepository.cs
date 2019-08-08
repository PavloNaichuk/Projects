using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePointData.Models;

namespace TicketSalePointData.Repositories
{
    public interface IUserRepository
    {
        User Authenticate(string login, string password);
        IEnumerable<User> GetAll();
        User GetById(int id);
        User Create(User user, string password);
        void Update(User user, string password = null);
        void Delete(int id);

    }
}
