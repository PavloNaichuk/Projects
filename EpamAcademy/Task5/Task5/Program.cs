using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data.SqlClient;
using System.Data.SqlServerCe;
using System.Linq;
using Task5.Data;
using Task5.Data.Model;
using Task5.Data.Services;

namespace Task5
{
    class Program
    {
        static IRepository<Product> rpProduct;
        static IRepository<Category> rpCategory;
        static IRepository<Vendor> rpVendor;
        static EntityDataService dataService;

        static void Main(string[] args)
        {
            while (true)
            {
                Console.Clear();
                Console.WriteLine("1. Select ADO/EF");
                Console.WriteLine("2. Select ADO/SQL");
                Console.WriteLine("Select a task option");

                if (Console.ReadKey().Key == ConsoleKey.D1)
                {
                    //EF
                    rpProduct = new EntityRepository<Product>(new DbContextEntity());
                    rpCategory = new EntityRepository<Category>(new DbContextEntity());
                    rpVendor = new EntityRepository<Vendor>(new DbContextEntity());
                    dataService = new EntityDataService(rpProduct, rpCategory, rpVendor);
                }
                else
                {
                    //ADO Classic
                    rpProduct = new SqlRepository<Product>(ConfigurationManager.ConnectionStrings["DEFAULT"].ConnectionString);
                    rpCategory = new SqlRepository<Category>(ConfigurationManager.ConnectionStrings["DEFAULT"].ConnectionString);
                    rpVendor = new SqlRepository<Vendor>(ConfigurationManager.ConnectionStrings["DEFAULT"].ConnectionString);
                }

                dataService = new EntityDataService(rpProduct, rpCategory, rpVendor);

                Step1();
                Step2();
                Step3();
                Step4();
                Step5();
                Step6();
                Step7();
                Step8();

                Console.Clear();
                Console.WriteLine("Exit: Y/N ?");
                if (Console.ReadKey().Key == ConsoleKey.Y) break;
            }
        }

        private static void Step1()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine("Get a list of products of a given category");
                    Console.WriteLine("Category List:");

                    var categoryList = dataService.CategoryList();
                    for (int I = 0; I < categoryList.Count; I++) Console.WriteLine($"{I + 1}: {categoryList[I].Name}");

                    Console.WriteLine();
                    Console.Write("Specify the category number:");
                    var categoryIndex = int.Parse(Console.ReadLine()) - 1;

                    Console.Clear();
                    Console.WriteLine("Step-1: Get a list of products of a given category");
                    Console.WriteLine();
                    Console.WriteLine($"Product category list '{categoryList[categoryIndex].Name}':");

                    var productsByCategory = dataService.ProductsByCategory(categoryList[categoryIndex]);
                    foreach (var product in productsByCategory) Console.WriteLine($"product: '{product.Name}'");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }
            }
        }

        private static void Step2()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine("Step-2: Identify all suppliers of goods of a given category");
                    Console.WriteLine();
                    Console.WriteLine("Category List: ");

                    var categoryList = dataService.CategoryList();
                    for (int I = 0; I < categoryList.Count; I++) Console.WriteLine($"{I + 1}: {categoryList[I].Name}");

                    Console.WriteLine();
                    Console.Write("Specify the category number: ");
                    var categoryIndex = int.Parse(Console.ReadLine()) - 1;

                    Console.Clear();
                    Console.WriteLine("Step-2: Identify all suppliers of goods of a given category ");
                    Console.WriteLine();
                    Console.WriteLine($"Product list for category '{categoryList[categoryIndex].Name}':");

                    var vendorsByCategory = dataService.VendorsByCategory(categoryList[categoryIndex]);
                    foreach (var vendor in vendorsByCategory) Console.WriteLine($"vendor: '{vendor.Name}'");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }
            }
        }

        private static void Step3()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine("Step-3: Determine the list of products of a given supplier");
                    Console.WriteLine();
                    Console.WriteLine("List of Suppliers: ");

                    var vendorList = dataService.VendorList();
                    for (int I = 0; I < vendorList.Count; I++) Console.WriteLine($"{I + 1}: {vendorList[I].Name}");

                    Console.WriteLine();
                    Console.Write("Specify the order number of the supplier: ");
                    var vendorIndex = int.Parse(Console.ReadLine()) - 1;

                    Console.Clear();
                    Console.WriteLine("Step-3: Determine the list of products of a given supplier");
                    Console.WriteLine();
                    Console.WriteLine($"Supplier list'{vendorList[vendorIndex].Name}':");

                    var productsByVendor = dataService.ProductsByVendor(vendorList[vendorIndex]);
                    foreach (var product in productsByVendor) Console.WriteLine($"product: '{product.Name}'");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }
            }
        }

        private static void Step4()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine("Step-4: Determine the suppliers whose products belong to the maximum number of categories");
                    Console.WriteLine();
                    Console.WriteLine($"List of suppliers: ");

                    var vendorList = dataService.VendorsByMaxCategory();
                    foreach (var vendor in vendorList) Console.WriteLine($"vendor: '{vendor.Name}'");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }

            }
        }

        private static void Step5()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine("Step 5: Search for products with a given price ");
                    Console.WriteLine();
                    Console.WriteLine("List of prices of goods: ");

                    var priceList = dataService.ProductList().Select(item => item.Price).Distinct().ToList();
                    for (int I = 0; I < priceList.Count; I++) Console.WriteLine($"{I + 1}: {priceList[I]}");

                    Console.WriteLine();
                    Console.Write("Indicate the order number of the specified price:");
                    var priceIndex = int.Parse(Console.ReadLine()) - 1;

                    Console.Clear();
                    Console.WriteLine("Step 5: Search for products with a given price ");
                    Console.WriteLine();
                    Console.WriteLine($"List of products with a given price '{priceList[priceIndex]}':");
                    var productsByPrice = dataService.ProductsByPrice(priceList[priceIndex]);
                    foreach (var product in productsByPrice) Console.WriteLine($"product: '{product.Name}'");

                    Console.ReadKey();
                    break;

                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }

            }
        }

        private static void Step6()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine("Step-6: search for products with a minimum price");
                    Console.WriteLine();
                    Console.WriteLine($"List of products with a minimum price: ");

                    var productsByPrice = dataService.ProductsByMinPrice();
                    foreach (var product in productsByPrice) Console.WriteLine($"product: '{product.Name}' price: {product.Price}");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }

            }
        }

        private static void Step7()
        {
            while (true)
            {
                try
                {
                    Console.Clear();

                    Console.WriteLine(" Step-7: search for products with the maximum price");
                    Console.WriteLine();
                    Console.WriteLine($"List of products with a maximum price: ");

                    var productsByPrice = dataService.ProductsByMaxPrice();
                    foreach (var product in productsByPrice) Console.WriteLine($"product: '{product.Name}' цена: {product.Price}");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }

            }
        }

        private static void Step8()
        {
            while (true)
            {
                try
                {
                    Console.Clear();
                    Console.WriteLine(" Step-8: search for suppliers from a given locality");
                    Console.WriteLine();
                    Console.WriteLine("List of locations:");
                    var locationList = dataService.VendorList().Select(item => item.Location).Distinct().ToList();
                    for (int I = 0; I < locationList.Count; I++) Console.WriteLine($"{I + 1}: {locationList[I]}");

                    Console.WriteLine();
                    Console.Write("Specify the number of the settlement: ");
                    var locationIndex = int.Parse(Console.ReadLine()) - 1;

                    Console.Clear();
                    Console.WriteLine("Step-8: search for suppliers from a given locality ");
                    Console.WriteLine();
                    Console.WriteLine($"list of locality providers '{locationList[locationIndex]}':");

                    var vendorsByLocation = dataService.VendorsByLocation(locationList[locationIndex]);
                    foreach (var vendor in vendorsByLocation) Console.WriteLine($"vendor: '{vendor.Name}'");

                    Console.ReadKey();
                    break;
                }
                catch (Exception error)
                {
                    Console.Clear();
                    Console.WriteLine(error.ToString());
                    Console.ReadKey();
                }

            }

        }

        private static void InitDatabase()
        {
            var rpProduct = new EntityRepository<Product>(new DbContextEntity());
            var rpCategory = new EntityRepository<Category>(new DbContextEntity());
            var rpVendor = new EntityRepository<Vendor>(new DbContextEntity());


            var connectionString = ConfigurationManager.ConnectionStrings["DEFAULT"].ConnectionString;
            SqlConnectionStringBuilder sqlConnectionBuilder = new SqlConnectionStringBuilder(connectionString);

            if (System.IO.File.Exists(sqlConnectionBuilder.DataSource)) return;
            new SqlCeEngine(connectionString).CreateDatabase();

            var categoryList = new List<Category>();
            categoryList.Add(new Category { Name = "Boots", ID = Guid.NewGuid() });
            categoryList.Add(new Category { Name = "T-shorts", ID = Guid.NewGuid() });
            categoryList.Add(new Category { Name = "Balls", ID = Guid.NewGuid() });
            rpCategory.Insert(categoryList);

            var vendorList = new List<Vendor>();
            vendorList.Add(new Vendor { Name = "Reebok", Location = "Ryzun", ID = Guid.NewGuid() });
            vendorList.Add(new Vendor { Name = "Addidas", Location = "Ryzun", ID = Guid.NewGuid() });
            vendorList.Add(new Vendor { Name = "Nike", Location = "Lviv", ID = Guid.NewGuid() });
            rpVendor.Insert(vendorList);

            var productList = new List<Product>();
            productList.Add(new Product { Name = "Black ball", Price = 1, CategoryId = categoryList[0].ID, VendorId = vendorList[0].ID, ID = Guid.NewGuid() });
            productList.Add(new Product { Name = "White boots", Price = 1, CategoryId = categoryList[0].ID, VendorId = vendorList[0].ID, ID = Guid.NewGuid() });
            productList.Add(new Product { Name = "Blue t-shirt", Price = 2, CategoryId = categoryList[1].ID, VendorId = vendorList[0].ID, ID = Guid.NewGuid() });
            productList.Add(new Product { Name = "Red ball", Price = 3, CategoryId = categoryList[1].ID, VendorId = vendorList[0].ID, ID = Guid.NewGuid() });
            productList.Add(new Product { Name = "Green t-shirt", Price = 3, CategoryId = categoryList[0].ID, VendorId = vendorList[1].ID, ID = Guid.NewGuid() });
            productList.Add(new Product { Name = "Yellow boots", Price = 2, CategoryId = categoryList[2].ID, VendorId = vendorList[2].ID, ID = Guid.NewGuid() });
            rpProduct.Insert(productList);

        }

    }
}
