using System;
using System.Collections.Generic;
using System.Data.Linq;
using System.Linq;
using System.Linq.Expressions;

namespace Task5.Data
{
    public class LinqRepository<TEntity> : IRepository<TEntity> where TEntity : class, new()
    {
        protected DataContext dbContext;


        public LinqRepository(DataContext DBContext)
        {
            dbContext = DBContext;
        }

        public TEntity Find(Expression<Func<TEntity, bool>> predicate)
        {
            return dbContext.GetTable<TEntity>().FirstOrDefault(predicate);
        }


        public IQueryable<TEntity> Select()
        {
            return dbContext.GetTable<TEntity>().AsQueryable();
        }


        public void Delete(TEntity entity)
        {
            dbContext.GetTable<TEntity>().DeleteOnSubmit(entity);
            dbContext.SubmitChanges();
        }


        public void Delete(IEnumerable<TEntity> entityList)
        {
            dbContext.GetTable<TEntity>().DeleteAllOnSubmit(entityList);
            dbContext.SubmitChanges();
        }

        public void Insert(TEntity entity)
        {
            dbContext.GetTable<TEntity>().InsertOnSubmit(entity);
            dbContext.SubmitChanges();
        }

        public void Insert(IEnumerable<TEntity> entityList)
        {
            dbContext.GetTable<TEntity>().InsertAllOnSubmit(entityList);
            dbContext.SubmitChanges();
        }

        public void Update(TEntity entity)
        {
            dbContext.SubmitChanges();
        }

        public void Update(IEnumerable<TEntity> entityList)
        {
            throw new NotSupportedException("метод массового обновления не поддерживается");
        }

        public void Clear()
        {
            var table = dbContext.GetTable<TEntity>();
            dbContext.GetTable<TEntity>().DeleteAllOnSubmit(table);
            dbContext.SubmitChanges();
        }
    }
}
