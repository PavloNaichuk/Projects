#include <iostream>

int main()
{
    // Task 1
    /*std::cout << "Task 1:\n";
    std::cout << "Enter seconds: ";
    int seconds;
    std::cin >> seconds;

    int hours = seconds / 3600;
    std::cout << "Hours: " << hours << "\n";
    seconds %= 3600;

    int minutes = seconds / 60;
    std::cout << "Minutes: " << minutes << "\n";

    seconds %= 60;

    std::cout << "Seconds: " << seconds << "\n"; 

    // Task 2
    std::cout << "Task 2:\n";

    std::cout << "Enter first number: ";
    double firstNumber;
    std::cin >> firstNumber;

    std::cout << "Enter second number: ";
    double secondNumber;
    std::cin >> secondNumber;

    std::cout << "Enter third number: ";
    double thirdNumber;
    std::cin >> thirdNumber;

    std::cout << "\n";
    double sum = firstNumber + secondNumber + thirdNumber;
    std::cout << "Sum numbers: " << sum << "\n";

    double product = firstNumber * secondNumber * thirdNumber;
    std::cout << "Product numbers: " << product << "\n";

    double average = sum / 3;
    std::cout << "Average numbers: " << average << "\n";*/


    // Task 3
    std::cout << "Task 3:\n";

    std::cout << "Enter first velue: ";
    double firstValue;
    std::cin >> firstValue;

    std::cout << "Enter second value: ";
    double secondValue;
    std::cin >> secondValue;
    std::cout << "\n";

    bool lessThan = firstValue < secondValue;
    bool equal = firstValue == secondValue;
    bool moreThan = firstValue > secondValue;
    bool lessOrEqual= firstValue <= secondValue;

    std::cout << "Less than: " << lessThan << "\n";
    std::cout << "Equal: " << equal << "\n";
    std::cout << "More than: " << moreThan << "\n";
    std::cout << "Less or equal: " << lessOrEqual << "\n";
    std::cout << "\n";


    std::cout << "Less than in string format: " << std::boolalpha << lessThan << "\n";
    std::cout << "Equal in string format: " << std::boolalpha << equal << "\n";
    std::cout << "More than in string format: " << std::boolalpha << moreThan << "\n";
    std::cout << "Less or equal in string format: " << std::boolalpha << lessOrEqual << "\n";

    // Task 4
    /*std::cout << "Task 4:\n";

    std::cout << "Enter height rectangle: ";
    int heightRectangle;
    std::cin >> heightRectangle;

    std::cout << "Enter width rectangle: ";
    int widthRectangle;
    std::cin >> widthRectangle;
    std::cout << "\n";

    int areaRectangle = heightRectangle * widthRectangle;
    std::cout << "Area rectangle: " << areaRectangle << "\n";

    int perimeterRectangle = 2 * (heightRectangle + widthRectangle);
    std::cout << "Perimeter rectangle: " << perimeterRectangle << "\n"; 

    // Task 5
    std::cout << "Task 5:\n";
    std::cout << "Enter radius circle : ";
    float radiusCircle;
    std::cin >> radiusCircle;
    std::cout << "\n";

    const float pi = 3.14f;
    float areaCircle = pi * (radiusCircle * radiusCircle);
    std::cout << "Area circle: " << areaCircle << "\n";

    float lengthCircle =  2 * pi * radiusCircle;
    std::cout << "Length circle: " << lengthCircle << "\n";*/


    return 0;
}