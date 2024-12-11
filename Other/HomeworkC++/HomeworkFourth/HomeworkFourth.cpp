#include <cmath>
#include <iostream>

int maxNumber(int num1, int num2) 
{ 
    return (num1 > num2) ? num1 : num2; 
}

int minNumber(int num1, int num2) 
{ 
    return (num1 < num2) ? num1 : num2; 
}

bool AreEqual(float v1, float v2) 
{
    const float epsilon = 0.001f;
    return (std::abs(v1 - v2) < epsilon);
}

enum class Month : int
{
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December,
    Count
};

int main()
{
    // Task 1
    /*std::cout << "Task 1:\n";

    std::cout << "Enter first number: ";
    int num1;
    std::cin >> num1;

    std::cout << "Enter second number: ";
    int num2;
    std::cin >> num2;

    std::cout << "Enter third number: ";
    int num3;
    std::cin >> num3;

    int maxNum = maxNumber(maxNumber(num1, num2), num3);

    std::cout << "Max number: " << maxNum << "\n";

    // Task 2
    std::cout << "Task 2:\n";

    std::cout << "Enter number: ";
    int number;
    std::cin >> number;

    //if ((number % 5 == 0) && (number % 11 == 0))
       // std::cout << "Divisible by these numbers\n";
    //else
       // std::cout << "Indivisible by these numbers\n";

    // Task 3
    std::cout << "Task 3:\n";
    std::cout << "Enter first triangle angle number: ";
    float angle1;
    std::cin >> angle1;

    std::cout << "Enter second triangle angle number: ";
    float angle2;
    std::cin >> angle2;

    std::cout << "Enter third triangle angle number: ";
    float angle3;
    std::cin >> angle3;
    
    float angleSum = angle1 + angle2 + angle3;

    if (AreEqual(angleSum, 180.0f))
        std::cout << "Angles form a triangle\n";
    else
        std::cout << "Angles do not form a triangle\n";

    // Task 4
    //std::cout << "Task 4:\n";

    // Task 5
    /*std::cout << "Task 5:\n";

    std::cout << "Enter first value: ";
    int value1;
    std::cin >> value1;

    std::cout << "Enter second value: ";
    int value2;
    std::cin >> value2;

    int minNum = minNumber(value1, value2);
    std::cout << "Min number: " << minNum << "\n";*/

    // Task 5
    std::cout << "Task 6:\n";

    std::cout << "Enter value: ";
    int value;
    std::cin >> value;
    if ((0 < value) && (value < static_cast<int>(Month::Count)))
    {
        const Month month = Month(value);
        switch (month)
        {
            case Month::January:
                std::cout << "January\n";
                break;
            case Month::February:
                std::cout << "February\n";
                break;
            case Month::March:
                std::cout << " March\n";
                break;
            case Month::April:
                std::cout << "April\n";
                break;
            case Month::May:
                std::cout << "May\n";
                break;
            case Month::June:
                std::cout << "June\n";
                break;
            case Month::July:
                std::cout << "July\n";
                break;
            case Month::August:
                std::cout << "August\n";
                break;
            case Month::September:
                std::cout << "September\n";
                break;
            case Month::October:
                std::cout << "October\n";
                break;
            case Month::November:
                std::cout << "November\n";
                break;
            case Month::December:
                std::cout << "December\n";
                break;
        }
    }
    else
    {
        std::cout << "Invalid input\n";
    }

    return 0;
}