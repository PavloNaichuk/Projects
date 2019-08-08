using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePointData.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public class ShowRepository : IShowRepository
    {
        private readonly DataBaseContext dataBaseContext;

        public ShowRepository(DataBaseContext context)
        {
            this.dataBaseContext = context;
        }

        public async Task Add(Show show)
        {
            dataBaseContext.Shows.Add(show);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task Remove(Show show)
        {
            dataBaseContext.Shows.Remove(show);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task<Show> Query(int id)
        {
            return  await dataBaseContext.Shows.FirstOrDefaultAsync(item => item.Id == id);
        }

        public async Task<IEnumerable<Show>> GetShowsSortedByDateForAllTheatres()
        {
            var query = from show in dataBaseContext.Shows
                        select new Show { Id = show.Id, Name = show.Name, TheatreId = show.TheatreId, Date = show.Date, Theatre = show.Theatre };
            query = query.OrderBy(date => date.Date);
            return await query.ToListAsync();
        }

        public async Task<IEnumerable<Show>> GetShowsSortedByDateForTheatre(int theatreId)
        {
            var query = from show in dataBaseContext.Shows
                        where
                        show.TheatreId == theatreId
                        select new Show { Id = show.Id, Name = show.Name, Date = show.Date };
            query = query.OrderBy(date => date.Date);
            return await query.ToListAsync();
        }
    }
}
