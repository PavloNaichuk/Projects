using Data.Repositories;
using SocialNetwork.Data.Context;
using SocialNetwork.Data.Models;
using System;
using System.Collections.Generic;
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

        public void AddFriendConection(int userId1, int userId2)
        {
            dataBaseContext.FriendInfo.Add(new FriendInfo() { UserId = userId1, FriendId = userId2 });
            dataBaseContext.FriendInfo.Add(new FriendInfo() { UserId = userId2, FriendId = userId1 });
            dataBaseContext.SaveChanges();
        }

        public void RemoveFriendConection(int userId1, int userId2)
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

            var query = from msg in dataBaseContext.MessageInfo
                        from userFriends in dataBaseContext.FriendInfo
                        where
                        userFriends.UserId == userId &&
                        msg.SenderId == userFriends.FriendId &&
                        msg.ReceiverId == userId
                        orderby msg.Date descending
                        select msg;

            var friendMsgList = query.ToList();

            var resultList = new List<MessageInfo>();
            var friendsGroupList = friendMsgList.GroupBy(item => item.SenderId);
            foreach (var group in friendsGroupList)
                resultList.Add(group.First());
            return resultList;
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
    }
}