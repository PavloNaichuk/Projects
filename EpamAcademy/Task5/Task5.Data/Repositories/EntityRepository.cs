using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Data.Entity.Core.Mapping;
using System.Data.Entity.Core.Metadata.Edm;
using System.Data.Entity.Infrastructure;
using System.Diagnostics;
using System.Linq;
using System.Linq.Expressions;

namespace Task5.Data
{
    public class EntityRepository<TEntity> : IRepository<TEntity> where TEntity : class, new()
    {

        public DbContext dbContext;


        public EntityRepository(DbContext DBContext)
        {
            dbContext = DBContext;
        }


        public TEntity Find(Expression<Func<TEntity, bool>> predicate)
        {
            return dbContext.Set<TEntity>().FirstOrDefault(predicate);
        }

        public IQueryable<TEntity> Select()
        {
            return dbContext.Set<TEntity>().AsQueryable();
        }

        public void Delete(TEntity entity)
        {
            dbContext.Set<TEntity>().Remove(entity);
            Commit();
        }

        public void Delete(IEnumerable<TEntity> entityList)
        {
            dbContext.Set<TEntity>().RemoveRange(entityList);
            Commit();
        }

        public void Insert(TEntity entity)
        {
            dbContext.Set<TEntity>().Add(entity);
            Commit();
        }

        public void Insert(IEnumerable<TEntity> entityList)
        {
            dbContext.Set<TEntity>().AddRange(entityList);
            dbContext.SaveChanges();
        }

        public void Update(TEntity entity)
        {
            if (dbContext.Entry<TEntity>(entity).State == EntityState.Detached)
                try
                {
                    dbContext.Set<TEntity>().Attach(entity);
                    dbContext.Entry<TEntity>(entity).State = EntityState.Unchanged;
                }
                catch (Exception ex) { Debug.Write(ex); }

            try { dbContext.Entry<TEntity>(entity).State = EntityState.Modified; } catch (Exception ex) { Debug.Write(ex); }
            Commit();
        }

        public void Update(IEnumerable<TEntity> entityList)
        {
            foreach (var entity in entityList)
            {
                if (dbContext.Entry<TEntity>(entity).State == EntityState.Detached)
                    try
                    {
                        dbContext.Set<TEntity>().Attach(entity);
                        dbContext.Entry<TEntity>(entity).State = EntityState.Unchanged;
                    }
                    catch (Exception ex) { Debug.Write(ex); }

                try { dbContext.Entry<TEntity>(entity).State = EntityState.Modified; } catch (Exception ex) { Debug.Write(ex); }

            }
            dbContext.SaveChanges();
        }

        public void Clear()
        {
            var tableName = GetTableName(typeof(TEntity), dbContext);
            dbContext.Database.ExecuteSqlCommand("delete from dbo." + tableName);
        }

        private DbContextTransaction transaction;
        public void Transaction(Action TransactionAction)
        {
            using (transaction = dbContext.Database.BeginTransaction())
            {
                try
                {
                    TransactionAction();
                    transaction.Commit();
                }
                catch (Exception) { transaction.Rollback(); }
            }

            transaction = null;
            Commit();
        }

        private void Commit()
        {
            if (transaction != null) return;
            dbContext.SaveChanges();
        }

        public string GetTableName(Type type, DbContext context)
        {
            var metadata = ((IObjectContextAdapter)context).ObjectContext.MetadataWorkspace;

            // Get the part of the model that contains info about the actual CLR types
            var objectItemCollection = ((ObjectItemCollection)metadata.GetItemCollection(DataSpace.OSpace));

            // Get the entity type from the model that maps to the CLR type
            var entityType = metadata
                    .GetItems<EntityType>(DataSpace.OSpace)
                    .Single(e => objectItemCollection.GetClrType(e) == type);

            // Get the entity set that uses this entity type
            var entitySet = metadata
                .GetItems<EntityContainer>(DataSpace.CSpace)
                .Single()
                .EntitySets
                .Single(s => s.ElementType.Name == entityType.Name);

            // Find the mapping between conceptual and storage model for this entity set
            var mapping = metadata.GetItems<EntityContainerMapping>(DataSpace.CSSpace)
                    .Single()
                    .EntitySetMappings
                    .Single(s => s.EntitySet == entitySet);

            // Find the storage entity set (table) that the entity is mapped
            var table = mapping
                .EntityTypeMappings.Single()
                .Fragments.Single()
                .StoreEntitySet;

            // Return the table name from the storage entity set
            return (string)table.MetadataProperties["Table"].Value ?? table.Name;
        }

    }
}
