#include <iostream>

int main()
{
    // Task 1
    std::cout << "Task 1:\n";
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
    std::cout << "\n";

    // Task 2
    std::cout << "Task 2:\n";

    std::cout << "Enter first number: ";
    float firstNumber;
    std::cin >> firstNumber;

    std::cout << "Enter second number: ";
    float secondNumber;
    std::cin >> secondNumber;

    std::cout << "Enter third number: ";
    float thirdNumber;
    std::cin >> thirdNumber;

    std::cout << "\n";
    float sum = firstNumber + secondNumber + thirdNumber;
    std::cout << "Sum numbers: " << sum << "\n";

    float product = firstNumber * secondNumber * thirdNumber;
    std::cout << "Product numbers: " << product << "\n";

    float average = sum / 3;
    std::cout << "Average numbers: " << average << "\n";
    std::cout << "\n";

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
    std::cout << "\n";

    // Task 4
    std::cout << "Task 4:\n";

    std::cout << "Enter rectangle height: ";
    int rectangleHeight;
    std::cin >> rectangleHeight;

    std::cout << "Enter rectangle width: ";
    int rectangleWidth;
    std::cin >> rectangleWidth;
    std::cout << "\n";

    int rectangleArea = rectangleHeight * rectangleWidth;
    std::cout << "Rectangle area: " << rectangleArea << "\n";

    int rectanglePerimeter = 2 * (rectangleHeight + rectangleWidth);
    std::cout << "Rectangle Perimeter: " << rectanglePerimeter << "\n";
    std::cout << "\n";

    // Task 5
    std::cout << "Task 5:\n";
    std::cout << "Enter circle radius: ";
    float circleRadius;
    std::cin >> circleRadius;
    std::cout << "\n";

    const float pi = 3.141592654f;
    float circleArea = pi * (circleRadius * circleRadius);
    std::cout << "Circle area: " << circleArea << "\n";

    float circleLength =  2 * pi * circleRadius;
    std::cout << "Circle length: " << circleLength << "\n";


    return 0;
}