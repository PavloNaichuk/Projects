using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public class TheatreRepository : ITheatreRepository
    {
        public async Task Add(Theatre theatre)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Theatres.Add(theatre);
                await dataBaseContext.SaveChangesAsync();
            }
        }

        public async Task Remove(Theatre theatre)
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                dataBaseContext.Theatres.Remove(theatre);
                await dataBaseContext.SaveChangesAsync();
            }
        }

        public async Task<Theatre> Query(int id)
        {
            using (var dataBaseContext = new DataBaseContext())
                return await dataBaseContext.Theatres.FirstOrDefaultAsync(item => item.Id == id);
        }

        public async Task<IEnumerable<TheatreSortInfo>> GetNames()
        {
            using (var dataBaseContext = new DataBaseContext())
            {
                var query = from theatre in dataBaseContext.Theatres
                            select new TheatreSortInfo{Id = theatre.Id, Name = theatre.Name };

                return await query.ToListAsync();
            }
        }
    }
}
