using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    interface IShowRepository
    {
        void Add(Show show);
        void Remove(Show show);
        Show Query(int id);
        Task<IEnumerable<ShowSortInfo>> GetShowsSortedByDateForAllTheatres();
        Task<IEnumerable<ShowSortInfo>> GetShowsSortedByDateForTheatre(int theatreId);
    }
}
