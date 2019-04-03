using System;
using System.Collections.Generic;
using System.Linq;

namespace Task5.Data.Services
{
    using Model;
    using System.Diagnostics;

    public class EntityDataService
    {
        private IRepository<Product> rpProduct;
        private IRepository<Vendor> rpVendor;
        private IRepository<Category> rpCategory;

        public EntityDataService(
            IRepository<Product> rpProduct,
            IRepository<Category> rpCategory,
            IRepository<Vendor> rpVendor)
        {
            this.rpProduct = rpProduct;
            this.rpVendor = rpVendor;
            this.rpCategory = rpCategory;
        }

        #region Category

        public List<Category> CategoryList()
        {
            var res = rpCategory.Select().ToList();
            return rpCategory.Select().ToList();
        }

        public Category CategoryById(Guid CategoryId)
        {
            return rpCategory.Find(item => item.ID == CategoryId);
        }

        public Category CategoryCreate(string Name)
        {
            var category = new Category { Name = Name };
            rpCategory.Insert(category);
            return category;
        }

        public void CategoryUpdate(Category Category)
        {
            if (Category == null) throw new ArgumentNullException(nameof(Category));
            rpCategory.Update(Category);
        }

        public void CategoryDelete(Category Category)
        {
            if (Category == null) throw new ArgumentNullException(nameof(Category));
            rpCategory.Delete(Category);
        }

        #endregion

        #region Product

        public List<Product> ProductList()
        {
            return rpProduct.Select().ToList();
        }

        public Product ProductById(Guid ProductId)
        {
            return rpProduct.Find(item => item.ID == ProductId);
        }

        public Product ProductCreate(string Name, int Price)
        {
            var product = new Product { Name = Name, Price = Price };
            rpProduct.Insert(product);
            return product;
        }

        public void ProductUpdate(Product Product)
        {
            if (Product == null) throw new ArgumentNullException(nameof(Product));
            rpProduct.Update(Product);
        }

        public void ProductDelete(Product Product)
        {
            if (Product == null) throw new ArgumentNullException(nameof(Product));
            rpProduct.Delete(Product);
        }

        public List<Product> ProductsByCategory(Category Category)
        {
            if (Category == null) throw new ArgumentNullException(nameof(Category));
            return rpProduct.Select().Where(item => item.CategoryId == Category.ID).ToList();
        }

        public List<Product> ProductsByVendor(Vendor Vendor)
        {
            if (Vendor == null) throw new ArgumentNullException(nameof(Vendor));
            return rpProduct.Select().Where(item => item.VendorId == Vendor.ID).ToList();
        }

        public List<Product> ProductsByPrice(int Price)
        {
            return rpProduct.Select().Where(item => item.Price == Price).ToList();
        }

        public List<Product> ProductsByMaxPrice()
        {
            var maxPrice = rpProduct.Select().Max(item => item.Price);
            return ProductsByPrice(maxPrice);
        }

        public List<Product> ProductsByMinPrice()
        {
            var minPrice = rpProduct.Select().Min(item => item.Price);
            return ProductsByPrice(minPrice);
        }

        #endregion

        #region Vendor

        public List<Vendor> VendorList()
        {
            return rpVendor.Select().ToList();
        }

        public Vendor VendorById(Guid VendorId)
        {
            return rpVendor.Find(item => item.ID == VendorId);
        }

        public Vendor VendorCreate(string Name, string Location)
        {
            var vendor = new Vendor { Name = Name, Location = Location };
            rpVendor.Insert(vendor);
            return vendor;
        }

        public void VendorUpdate(Vendor Vendor)
        {
            if (Vendor == null) throw new ArgumentNullException(nameof(Vendor));
            rpVendor.Update(Vendor);
        }

        public void VendorDelete(Vendor Vendor)
        {
            if (Vendor == null) throw new ArgumentNullException(nameof(Vendor));
            rpVendor.Delete(Vendor);
        }

        public List<Vendor> VendorsByCategory(Category Category)
        {
            if (Category == null) throw new ArgumentNullException(nameof(Category));
            var products = ProductsByCategory(Category);
            var vendors = products.Select(item => item.VendorId).Distinct().ToList();

            return rpVendor.Select().Where(item => vendors.Contains(item.ID)).ToList();
        }
        
        public List<Vendor> VendorsByLocation(string Location)
        {
            return rpVendor.Select().Where(item => item.Location == Location).ToList();
        }

        public List<Vendor> VendorsByMaxCategory()
        {
            var vendorsCategory = new List<(Vendor vendor, int category)>();
            var vendors = rpVendor.Select();

            foreach (var vendor in vendors)
            {
                var products = ProductsByVendor(vendor);
                var categoryCount = products.Select(item => item.CategoryId).Distinct().Count();
                vendorsCategory.Add((vendor, categoryCount));
            }
            var maxProductsCategory = vendorsCategory.Max(item => item.category);

            return vendorsCategory.Where(item => item.category == maxProductsCategory).Select(item => item.vendor).ToList();
        }

        #endregion
    }
}
