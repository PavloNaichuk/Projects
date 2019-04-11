using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace SocialNetwork.Data.Models
{
    [Table("Messages")]
    public class MessageInfo
    {
        public MessageInfo() { }
        public int Id { get; set; }
        public int ConversationId { get; set; }
        public int SenderId { get; set; }
        public int ReceiverId { get; set; }
        public string Text { get; set; }
        public DateTime Date { get; set; }
        public bool Unread { get; set; }
    }
}
