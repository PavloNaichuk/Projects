using Microsoft.AspNetCore.Identity;
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

        public Task<User> Authenticate(string login, string password)
        {
            throw new NotImplementedException();
        }
        public Task Add(User user)
        {
            throw new NotImplementedException();
        }

        public Task<User> AuthenticateExternal(int id)
        {
            throw new NotImplementedException();
        }

        public Task<User> AddExternal(int id, string name, string email)
        {
            throw new NotImplementedException();
        }
    }
}
