using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public class ShowRepository : IShowRepository
    {
        public async Task Add(Show show)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Shows.Add(show);
                await dataBaseContext.SaveChangesAsync();
            }
        }

        public async Task Remove(Show show)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Shows.Remove(show);
                await dataBaseContext.SaveChangesAsync();
            }
        }

        public async Task<Show> Query(int id)
        {
            using (var dataBaseContext = new DataBaseContext())
                return  await dataBaseContext.Shows.FirstOrDefaultAsync(item => item.Id == id);
        }

        public async Task<IEnumerable<ShowSortInfo>> GetShowsSortedByDateForAllTheatres()
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                            select new ShowSortInfo { Id = show.Id, Name = show.Name,  TheatreId  = show.TheatreId, Date = show.Date,  TheatreName = show.Theatre.Name };
                query.OrderByDescending(date => date.Date);
                return await query.ToListAsync();
            }
        }

        public async Task<IEnumerable<ShowSortInfo>> GetShowsSortedByDateForTheatre(int theatreId)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from show in dataBaseContext.Shows
                            where
                            show.TheatreId == theatreId
                            select new  ShowSortInfo { Id = show.Id, Name = show.Name, Date = show.Date };
                query.OrderByDescending(date => date.Date);
                return await query.ToListAsync();
            }
        }
    }
}
