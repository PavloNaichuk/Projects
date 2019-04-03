using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace Task5.Data
{
    public class SqlQueryProvider : IQueryProvider
    {
        public IQueryable CreateQuery(Expression expression)
        {
            try { return (IQueryable)Activator.CreateInstance(this.GetType().MakeGenericType(expression.Type), new object[] { this, expression }); }
            catch (System.Reflection.TargetInvocationException tie) { throw tie.InnerException; }
        }


        public IQueryable<TResult> CreateQuery<TResult>(Expression expression)
        {
            return new QueryableResult<TResult>(this, expression);
        }

        public object Execute(Expression expression)
        {
            throw new NotSupportedException();
        }

        public TResult Execute<TResult>(Expression expression)
        {
            MethodCallExpression whereExpression = (MethodCallExpression)expression;
            LambdaExpression lambdaExpression = (LambdaExpression)((UnaryExpression)(whereExpression.Arguments[1])).Operand;
            //анализ дерева выражений
            //... 

            return default(TResult);
        }
    }

    public class QueryableResult<TResult> : IQueryable<TResult>
    {
        public Type ElementType { get; private set; }
        public Expression Expression { get; private set; }

        public IQueryProvider Provider { get; private set; }

        public QueryableResult(IQueryProvider provider, Expression expression)
        {
            this.Provider = provider;
            this.Expression = expression ?? Expression.Constant(this);
            this.ElementType = expression == null ? typeof(TResult) : expression.Type;
        }


        public IEnumerator<TResult> GetEnumerator()
        {
            return Provider.Execute<IEnumerator<TResult>>(Expression);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return Provider.Execute<IEnumerator>(Expression);
        }
    }
}
