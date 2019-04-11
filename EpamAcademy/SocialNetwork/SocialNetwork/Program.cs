using System;
using Data.Repositories;

namespace SocialNetwork
{
    class Program
    {
        static void Main(string[] args)
        {
            UserRepository userRepository = new UserRepository();
            userRepository.ConsoleWrite();
            //MessageRepositories messageRepositories = new MessageRepositories();
            //messageRepositories.ConsoleWrite();
            //FriendRepository friendRepository = new FriendRepository();
            //friendRepository.ConsoleWrite();
        }

    }
}
