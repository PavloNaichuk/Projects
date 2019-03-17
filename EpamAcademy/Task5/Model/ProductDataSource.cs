using Model.Models;
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Model
{
    public class ProductDataSource : IDataSource<Product>
    {
        private readonly SqlDataAdapter sqlDataAdapter; 

        public ProductDataSource()
        {
            sqlDataAdapter = new SqlDataAdapter();
        }

        public void Delete(int id)
        {
            const string sql = @"delete from Product 
                                 where id = @id";
            SqlCommand command = new SqlCommand(sql);

            sqlDataAdapter.DeleteCommand = command;
            //sqlDataAdapter.Update();
        }

        public List<Product> GetAll()
        {
            throw new NotImplementedException();
        }

        public Product GetById(int id)
        {
            throw new NotImplementedException();
        }

        public Product Insert(Product newObject)
        {
            throw new NotImplementedException();
        }

        public Product Update(Product updatedObject)
        {
            throw new NotImplementedException();
        }
    }
}
