using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    class TheatreRepository : ITheatreRepository
    {
        private readonly DataBaseContext dataBaseContext;

        public TheatreRepository(DataBaseContext context)
        {
            this.dataBaseContext = context;
        }

        public async Task Add(Theatre theatre)
        {
            dataBaseContext.Theatres.Add(theatre);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task Remove(Theatre theatre)
        {
            dataBaseContext.Theatres.Remove(theatre);
            await dataBaseContext.SaveChangesAsync();
        }

        public async Task<Theatre> Query(int id)
        {
            return await dataBaseContext.Theatres.FirstOrDefaultAsync(item => item.Id == id);
        }

        public async Task<IEnumerable<TheatreSortInfo>> GetNames()
        {
            var query = from theatre in dataBaseContext.Theatres
                        select new TheatreSortInfo { Id = theatre.Id, Name = theatre.Name };

            return await query.ToListAsync();
        }
    }
}
