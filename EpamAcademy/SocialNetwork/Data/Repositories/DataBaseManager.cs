using Data.Repositories;
using SocialNetwork.Data.Context;
using SocialNetwork.Data.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace SocialNetwork
{
    public sealed class DataBaseManager
    {
        public static readonly DataBaseManager Instance = new DataBaseManager();
        private DataBaseManager() { }

        private DataBaseContext dataBaseContext = new DataBaseContext();

        public void AddUser(UserInfo userInfo)
        {
            dataBaseContext.UserInfo.Add(userInfo);
            dataBaseContext.SaveChanges();
        }

        public void RemoveUser(UserInfo userInfo)
        {
            dataBaseContext.UserInfo.Remove(userInfo);
            dataBaseContext.SaveChanges();
        }

        public void AddFriendConnection(int userId1, int userId2)
        {
            dataBaseContext.FriendInfo.Add(new FriendInfo() { UserId = userId1, FriendId = userId2 });
            dataBaseContext.FriendInfo.Add(new FriendInfo() { UserId = userId2, FriendId = userId1 });
            dataBaseContext.SaveChanges();
        }

        public void RemoveFriendConnection(int userId1, int userId2)
        {
            dataBaseContext.FriendInfo.Remove(new FriendInfo() { UserId = userId1, FriendId = userId2});
            dataBaseContext.FriendInfo.Remove(new FriendInfo() { UserId = userId2, FriendId = userId1 });
            dataBaseContext.SaveChanges();
        }

        public void AddMessage(MessageInfo messageInfo)
        {
           dataBaseContext.MessageInfo.Add(messageInfo);
           dataBaseContext.SaveChanges();
        }

        public int CountUnreadMessages(int userId)
        {
            return dataBaseContext.MessageInfo.Count(item => item.ReceiverId == userId && item.Unread);
        }

        public void MarkMessageAsRead(int messageId)
        {
            var query = from msg in dataBaseContext.MessageInfo
                          where msg.Id == messageId
                          select msg;

            if (query.Count() == 1)
            {
                query.First().Unread = false;
                dataBaseContext.SaveChanges();
            }
            else
                Debug.Assert(false, "Unknown messageId");
        }

        public IList<MessageInfo> QueryMessagesFromConversation(int userId1, int userId2)
        {
            int conversationId = QueryConversationId(userId1, userId2);

            var query = from msg in dataBaseContext.MessageInfo
                        where
                        msg.ConversationId == conversationId
                        select msg;

            return query.ToList();
        }

        public IList<MessageInfo> QueryLastMessageFromEachConversation(int userId)
        {
            var query = from message in dataBaseContext.MessageInfo
                        where
                        message.ReceiverId == userId || message.SenderId == userId
                        group message by message.ConversationId into messages
                        select new
                        {
                            messages.Key,
                            Message = messages.OrderByDescending(m => m.Id).FirstOrDefault()
                        };
            return query.Select(q => q.Message).ToList();
                        
        }

        public int QueryConversationId(int userId1, int userId2)
        {
            if (userId1 <= 0)
                throw new ArgumentException("Invalid parameter. The parameter should be greater than 0", "userId1");

            if (userId2 <= 0)
                throw new ArgumentException("Invalid parameter. The parameter should be greater than 0", "userId2");

            if (userId1 == userId2)
                throw new ArgumentException("Invalid parameters. The parameters are not alloved to be equal", "");

            // There is no need to keep a separate table for conversation ids between two users.
            // We can always generate a unique integer for a pair of integers on the application side.
            // This allows us to decrease number of requests to database and speed up search. 
            return MathHelpers.GenerateUniqueNumberFromPair(Math.Min(userId1, userId2), Math.Max(userId1, userId2));
        }

        public IList<FriendInfo> QueryAllFriendIds(int userId)
        {
            var query = from friend in dataBaseContext.FriendInfo
                        where
                        friend.UserId == userId
                        select friend;
            return query.ToList();
        }

         public IList<UserInfo> QueryAllFriends(int userId)
         {
            var query = from friend in dataBaseContext.FriendInfo
                        join userInfo in dataBaseContext.UserInfo on friend.FriendId equals userInfo.Id
                        where
                            friend.FriendId == userId
                        select userInfo;
            return query.ToList();
         }

        public UserInfo QueryUserInfo(string login)
        {
            return dataBaseContext.UserInfo.FirstOrDefault(item => item.Login == login);
        }

        public void ClearDB()
        {
            var messages = from c in dataBaseContext.MessageInfo select c;
            dataBaseContext.MessageInfo.RemoveRange(messages);

            var friends = from c in dataBaseContext.FriendInfo select c;
            dataBaseContext.FriendInfo.RemoveRange(friends);

            var userInfo = from c in dataBaseContext.UserInfo select c;
            dataBaseContext.UserInfo.RemoveRange(userInfo);
            dataBaseContext.SaveChanges();
        }
    }
}