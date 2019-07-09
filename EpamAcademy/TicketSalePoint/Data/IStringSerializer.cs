using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace TicketSalePoint.Data
{
    public interface IStringSerializer<TEntity>
    {
        TEntity Deserialize(string strEntity);
        string Serialize(TEntity Entity);
    }
}
