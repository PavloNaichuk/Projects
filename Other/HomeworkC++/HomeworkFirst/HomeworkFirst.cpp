#include <iostream>
#include <cstdlib>

int main()
{
    std::cout << "I love C++!\n";


    std::cout << "Enter number right triangle: ";
    int number;
    std::cin >> number;
    if (number > 1)
    {
        for (int row = 0; row <= number; ++row)
        {
            for (int col = 0; col < row; ++col)
                std::cout << "*";
            std::cout << "\n";
        }
    }
    else
        std::cout << "Sorry number must be greater than 1 \n";
   
    return (0);
}

