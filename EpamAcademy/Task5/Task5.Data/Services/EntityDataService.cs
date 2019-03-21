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
            return rpCategory.Select().ToList();
        }

        public Category CategoryById(Guid CategoryId)
        {
            return rpCategory.Find(item => item.ID == CategoryId);
        }

        #endregion

        #region Product

        public List<Product> ProductList()
        {
            return rpProduct.Select().ToList();
        }

        public List<Product> ProductsByCategory(Category Category)
        {
            return rpProduct.Select().Where(item => item.CategoryId == Category.ID).ToList();
        }

        public List<Product> ProductsByVendor(Vendor Vendor)
        {
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

        public List<Vendor> VendorsByCategory(Category Category)
        {
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
