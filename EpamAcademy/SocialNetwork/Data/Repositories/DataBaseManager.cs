using SocialNetwork.Data.Context;
using SocialNetwork.Data.Models;
using System;
using System.Collections.Generic;

namespace SocialNetwork
{
    public class DataBaseManager
    {
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

        public void AddFriend(FriendInfo friendInfo)
        {
            dataBaseContext.FriendInfo.Add(friendInfo);
            dataBaseContext.SaveChanges();
        }

        public void RemoveFriend(FriendInfo friendInfo)
        {
            dataBaseContext.FriendInfo.Remove(friendInfo);
            dataBaseContext.SaveChanges();
        }

        public void SendMessage(MessageInfo messageInfo)
        {
           dataBaseContext.MessageInfo.Add(messageInfo);
           dataBaseContext.SaveChanges();
        }

        public bool CheckIfUnreadMessagesArePresent()
        {
            return true;
        }

       /* public IList<UserInfo> FetchMessagesFromConversation(int userID1, int userID2)
        {
           
        }

        public IList<MessageInfo> FetchLastMessageFromEachConversation((int userID)) { }*/

        public void PrintUserInfo()
        {
            var users = dataBaseContext.UserInfo;
            Console.WriteLine("List:");
            foreach (UserInfo userInfo in users)
            {
                Console.WriteLine("{0}. \n{1} \n {2}\n {3} \n {4} \n {5} \n {6}", userInfo.Id, userInfo.Login, userInfo.Password,
                userInfo.FirstName, userInfo.LastName, userInfo.Email, userInfo.RegistrationDate);
            }
         }

        public void PrintMessageInfo()
        {
            var messages = dataBaseContext.MessageInfo;
            Console.WriteLine("List:");
            foreach (MessageInfo messageInfo in messages)
            {
                Console.WriteLine("{0}. \n{1} \n {2}\n {3} \n {4} \n {5}", messageInfo.Id, messageInfo.SenderId, messageInfo.ReceiverId,
                    messageInfo.Message, messageInfo.Date, messageInfo.WasRead);
            }
        }

        public void PrintFriendInfo()
        {
            var friends = dataBaseContext.FriendInfo;
            Console.WriteLine("List:");
            foreach (FriendInfo friendInfo in friends)
            {
                Console.WriteLine("{0}.\n{1} \n {2}\n", friendInfo.UserId, friendInfo.FriendId);
            }
        }
    }
}