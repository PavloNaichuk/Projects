using System;
using Data.Repositories;

namespace SocialNetwork
{
    class Program
    {
        static void Main(string[] args)
        {
            UserRepository userRepository = new UserRepository();
            userRepository.temp();
        }

    }
}
