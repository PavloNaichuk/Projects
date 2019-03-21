using Task5.Data.Model;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Data.Entity.Core.Mapping;
using System.Data.Entity.Core.Metadata.Edm;
using System.Data.Entity.Infrastructure;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;
using System.Data.SqlServerCe;
using System.Collections;

namespace Task5.Data
{
    public class SqlRepository<TEntity> : IRepository<TEntity> where TEntity : class, new()
    {
        public string connectionString { get; }

        public SqlRepository(string connectionString)
        {
            this.connectionString = connectionString;
        }

        public TEntity Find(Expression<Func<TEntity, bool>> predicate)
        {
            return GetTable().AsQueryable().FirstOrDefault(predicate);
        }

        public IQueryable<TEntity> Select()
        {
            return GetTable().AsQueryable();
    
        }

        public void Delete(TEntity enitty)
        {
            throw new NotImplementedException();
        }

        public void Delete(IEnumerable<TEntity> entityList)
        {
            foreach (var entity in entityList) Delete(entity);
        }

        public void Insert(TEntity entity)
        {
            throw new NotImplementedException();
        }

        public void Insert(IEnumerable<TEntity> entityList)
        {
            foreach (var entity in entityList) Insert(entity);
        }

        public void Update(TEntity entity)
        {
            throw new NotImplementedException();

        }

        public void Update(IEnumerable<TEntity> entityList)
        {
            foreach (var entity in entityList) Update(entity);
        }

        public void Clear()
        {
            ExecuteNonQuery($"delete from {GetTableName()}");
        }

        private void ExecuteNonQuery(string Sql)
        {
            using (var sqlConnection = new SqlCeConnection(connectionString))
            {
                SqlCeCommand sqlCommand = new SqlCeCommand(Sql, sqlConnection);
                sqlCommand.ExecuteNonQuery();
            }
        }

        private List<TEntity> GetTable(string where = null)
        {
            var query = $"select * from {GetTableName()} {where}";

            var dataTable = new DataTable();
            using (var sqlConnection = new SqlConnection(connectionString))
            {
                SqlDataAdapter sqlDA = new SqlDataAdapter(query, sqlConnection);
                sqlDA.Fill(dataTable);
            }

            var entityList = new List<TEntity>();
            foreach (DataRow row in dataTable.Rows)
            {
                var entity = new TEntity();
                foreach (DataColumn column in dataTable.Columns)
                {
                    var value = row[column.ColumnName];
                    typeof(TEntity).GetProperty(column.ColumnName).SetValue(entity, value);
                }

                entityList.Add(entity);
            }

            return entityList;
        }

        private string GetTableName()
        {
            var attributeList = typeof(TEntity).GetCustomAttributes(typeof(TableAttribute), false);
            if (attributeList.Length == 0) return nameof(TEntity) + "s";
            return (attributeList[0] as TableAttribute).Name;
        }

        private string GetKeyName()
        {
            var attributeList = typeof(TEntity).GetCustomAttributes(typeof(KeyAttribute), false);
            if (attributeList.Length == 0) return null;
            return (attributeList[0] as KeyAttribute).ToString();
        }
    }


}
