using System.Collections.Generic;
using System.Threading.Tasks;
using TicketSalePoint.Models;

namespace TicketSalePoint.Repositories
{
    public interface IShowRepository
    {
        Task Add(Show show);
        Task Remove(Show show);
        Task<Show> Query(int id);
        Task<IEnumerable<Show>> GetShowsSortedByDateForAllTheatres();
        Task<IEnumerable<Show>> GetShowsSortedByDateForTheatre(int theatreId);
    }
}
