using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Task5.Data
{
    public interface IRepository<T>
    {
        T Find(Expression<Func<T, bool>> predicate);
        IQueryable<T> Select();
        void Update(T Entity);
        void Insert(T Entity);
        void Insert(IEnumerable<T> EntityList);
        void Update(IEnumerable<T> EntityList);
        void Delete(T Entity);
        void Clear();
    }
}
