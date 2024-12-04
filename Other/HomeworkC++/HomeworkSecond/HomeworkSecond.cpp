#include <iostream>
#include <algorithm>
#include <iomanip> 
#include <array>

enum class WeekDay
{
    Monday = 1, 
    Tuesday,
    Wednesday,
    Thursday,
    Friday, 
    Saturday,
    Sunday,
    Count
};

int main()
{
    // Task 1
    std::cout << "Task 1:\n";
    std::cout << "Enter numberFirst: ";
    int numberFirst;
    std::cin >> numberFirst;

    std::cout << "Enter numberSecond: ";
    int numberSecond;
    std::cin >> numberSecond;

    std::swap(numberFirst, numberSecond);

    std::cout << "\n";
    std::cout << "Number first: " << numberFirst << std::endl;
    std::cout << "Number second: " << numberSecond << std::endl;
    std::cout << "\n";

    // Task 2
    std::cout << "Task 2:\n";
    double dValue = 33677.4;
    std::cout << "Double value: " << dValue << std::endl;

    // Used static_cast
    int staticCastNumber = static_cast<int>(dValue);
    std::cout << "Integer used static_cast number: " << staticCastNumber << std::endl;

    // Used C-style
    int cStyleNumber = (int)dValue;
    std::cout << "Integer used C-style number: " << cStyleNumber << std::endl;

    std::cout <<"Manipulators value:" << std::setw(20) << std::right << std::setfill('$') << dValue << std::endl;
    std::cout << "\n";
    // Task 3
    std::cout << "Task 3:\n";
    double doubleValue = 3454.12345678910111213;
    std::cout << "Double value two number after coma: " << std::fixed << std::setprecision(2) << doubleValue << std::endl;
    std::cout << "Double value five number after coma: " << std::fixed << std::setprecision(5) << doubleValue << std::endl;
    std::cout << "Double value ten number after coma: " << std::fixed << std::setprecision(10) << doubleValue << std::endl;
    std::cout << "\n";
    std::cout << "Double value standard: " <<  doubleValue << std::endl;
    std::cout << "Double value eNotation: " << std::scientific << doubleValue << std::endl;
    std::cout << "\n";

    // Task 4
    std::cout << "Task 4:\n";
    for (int index = (int)WeekDay::Monday; index < (int)WeekDay::Count; ++index)
    {
        std::cout << "Numerical value of each day of the week: " << index << std::endl;
    }
    std::cout << "\n";
  
    // Task 5
    std::cout << "Task 5:\n";
    std::cout << "Enter number: ";
    int numberInteger;
    std::cin >> numberInteger;
    bool booleanValue = (bool)numberInteger;
    std::cout << "Boolean value:  " << std::boolalpha << booleanValue << std::endl;


    return 0;
}

