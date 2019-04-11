using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SocialNetwork.Data.Models
{
    [Table("Friends")]
    public class FriendInfo
    {
        public FriendInfo() { }
        public int Id { get; set; }
        public int UserId { get; set; }
        public int FriendId { get; set; }

    }
}
