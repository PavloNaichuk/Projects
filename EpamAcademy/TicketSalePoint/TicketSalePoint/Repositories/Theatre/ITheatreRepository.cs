using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Context;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    interface ITheatreRepository
    {
        void Add(Theatre theatre);
        void Remove(Theatre theatre);
        Theatre Query(int id);
        Task<IEnumerable<TheatreSortInfo>> GetNames();
    }
}

      

