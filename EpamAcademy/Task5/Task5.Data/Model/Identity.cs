using System;
using System.ComponentModel.DataAnnotations;

namespace Task5.Data.Model
{
    public class Identity<TKey> where TKey : IEquatable<TKey>
    {
        [Key]
        public TKey ID { get; set; }
    }
}
