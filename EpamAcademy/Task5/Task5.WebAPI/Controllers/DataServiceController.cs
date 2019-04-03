using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using System.Threading.Tasks;

namespace Task5.WebAPI.Controllers
{
    using global::Task5.Data.Model;
    using global::Task5.Data.Services;
    using Swashbuckle.Swagger.Annotations;

    namespace Task5.WebAPI.Controllers
    {
        public class DataServiceController : ApiController
        {
            private readonly EntityDataService dataService;

            public DataServiceController(EntityDataService dataService)
            {
                this.dataService = dataService;
            }

            #region Category
            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpGet]
            [Route("api/categories")]
            public IEnumerable<Category> CategoryList()
            {
                return dataService.CategoryList();
            }

            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpGet]
            [Route("api/categories/{CategoryId}")]
            public Category CategoryById(Guid CategoryId)
            {
                return dataService.CategoryById(CategoryId);
            }

            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpPost]
            [Route("api/categories")]
            public Category CategoryCreate(string Name)
            {
                return dataService.CategoryCreate(Name);
            }

            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpPut]
            [Route("api/categories/{CategoryId}")]
            public void CategoryUpdate(Guid CategoryId, string Name)
            {
                var category = dataService.CategoryById(CategoryId);
                category.Name = Name;
                dataService.CategoryUpdate(category);
            }

            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpDelete]
            [Route("api/categories/{CategoryId}")]
            public void CategoryDelete(Guid CategoryId)
            {
                var category = dataService.CategoryById(CategoryId);
                dataService.CategoryDelete(category);
            }


            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpGet]
            [Route("api/categories/{CategoryId}/products")]
            public IEnumerable<Product> ProductsByCategory(Guid CategoryId)
            {
                var category = dataService.CategoryById(CategoryId);
                return dataService.ProductsByCategory(category);
            }


            [SwaggerOperation(Tags = new[] { "Category" })]
            [HttpGet]
            [Route("api/categories/{CategoryId}/products/suppliers")]
            public IEnumerable<Vendor> VendorsByCategory(Guid CategoryId)
            {
                var category = dataService.CategoryById(CategoryId);
                return dataService.VendorsByCategory(category);
            }

            #endregion

            #region Vendors

            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpGet]
            [Route("api/suppliers")]
            public IEnumerable<Vendor> VendorList()
            {
                return dataService.VendorList();
            }

            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpGet]
            [Route("api/suppliers/{VendorId}")]
            public Vendor VendorById(Guid VendorId)
            {
                return dataService.VendorById(VendorId);
            }

            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpPost]
            [Route("api/suppliers")]
            public Vendor VendorCreate(string Name, string Location)
            {
                return dataService.VendorCreate(Name, Location);
            }

            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpPut]
            [Route("api/suppliers/{VendorId}")]
            public void VendorUpdate(Guid VendorId, string Name, string Location)
            {
                var vendor = dataService.VendorById(VendorId);
                vendor.Name = Name;
                vendor.Location = Location;
                dataService.VendorUpdate(vendor);
            }

            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpDelete]
            [Route("api/suppliers/{VendorId}")]
            public void VendorDelete(Guid VendorId)
            {
                var vendor = dataService.VendorById(VendorId);
                dataService.VendorDelete(vendor);
            }



            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpGet]
            [Route("api/suppliers/bylocation/{Location}")]
            public IEnumerable<Vendor> VendorsByCategory(string Location)
            {
                return dataService.VendorsByLocation(Location);
            }



            [SwaggerOperation(Tags = new[] { "Vendors" })]
            [HttpGet]
            [Route("api/suppliers/{VendorId}/products")]
            public IEnumerable<Product> ProductsByVendor(Guid CategoryId)
            {
                var vendor = dataService.VendorById(CategoryId);
                return dataService.ProductsByVendor(vendor);
            }

            #endregion

            #region Product

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpGet]
            [Route("api/products ")]
            public IEnumerable<Product> ProductList()
            {
                return dataService.ProductList();
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpGet]
            [Route("api/products/{ProductId}")]
            public Product ProductById(Guid ProductId)
            {
                return dataService.ProductById(ProductId);
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpPost]
            [Route("api/products")]
            public Product ProductCreate(string Name, int Price)
            {
                return dataService.ProductCreate(Name, Price);
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpPut]
            [Route("api/products/{ProductId}")]
            public void ProductUpdate(Guid ProductId, string Name, int Price, Guid CategoryId, Guid VendorId)
            {
                var product = dataService.ProductById(ProductId);
                product.Name = Name;
                product.Price = Price;
                product.VendorId = VendorId;
                product.CategoryId = CategoryId;
                dataService.ProductUpdate(product);
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpDelete]
            [Route("api/products/{ProductId}")]
            public void ProductDelete(Guid ProductId)
            {
                var product = dataService.ProductById(ProductId);
                dataService.ProductDelete(product);
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpGet]
            [Route("api/products/byprice/{Price}")]
            public IEnumerable<Product> ProductsByPrice(int Price)
            {
                return dataService.ProductsByPrice(Price);
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpGet]
            [Route("api/products/bymaxprice")]
            public List<Product> ProductsByMaxPrice()
            {
                return dataService.ProductsByMaxPrice();
            }

            [SwaggerOperation(Tags = new[] { "Product" })]
            [HttpGet]
            [Route("api/products/byminprice")]
            public List<Product> ProductsByMinPrice()
            {
                return dataService.ProductsByMinPrice();
            }
            #endregion
        }
    }

}
