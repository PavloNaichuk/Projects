using Model.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EFModel
{
    public class EFHelper
    {
        private readonly Context context;

        public void Delete(int id)
        {
            var p = context.products.Find(id);
            if (p == null)
                throw new Exception("OPS");
            context.products.Remove(p);
            context.SaveChanges();
        }

        public List<Product> GetAll()
        {
            return context.products.ToList();
        }

        public Product GetById(int id)
        {
            return context.products.FirstOrDefault(p => p.ID == id);
        }

        public Product Insert(Product newObject)
        {
            //TODO perevirki
            var createdProduct = context.products.Add(newObject);
            context.SaveChanges();
            return createdProduct;
        }

        public Product Update(Product updatedObject)
        {
            //TODO perevirki
            var product = context.products.FirstOrDefault(p => p.ID == updatedObject.ID);
            product.Name = updatedObject.Name;
            product.ProducerId = updatedObject.ProducerId;
            product.CategoryId = updatedObject.CategoryId;
            context.SaveChanges();
            return product;
        }
    }
}
