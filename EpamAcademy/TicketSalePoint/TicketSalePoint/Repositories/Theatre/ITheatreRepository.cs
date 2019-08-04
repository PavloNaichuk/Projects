using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public interface ITheatreRepository
    {
        Task Add(Theatre theatre);
        Task Remove(Theatre theatre);
        Task<Theatre> Query(int id);
        Task<IEnumerable<TheatreSortInfo>> GetNames();
    }
}

      

