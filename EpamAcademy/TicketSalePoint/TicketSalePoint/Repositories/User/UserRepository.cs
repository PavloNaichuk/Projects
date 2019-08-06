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
        private UserManager<ApplicationUser> userManager;
        private SignInManager<ApplicationUser> signInManager;

        private readonly DataBaseContext dataBaseContext;

        public UserRepository(DataBaseContext context)
        {
            this.dataBaseContext = context;
        }
    }
}
