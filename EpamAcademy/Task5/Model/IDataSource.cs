using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Model
{
    public interface IDataSource<T>
    {
        T GetById(int id);
        List<T> GetAll();
        void Delete(int id);
        T Insert(T newObject);
        T Update(T updatedObject);
    }
}
