#include "Vector2f.h"
#include "Vector3f.h"
#include "Matrix2f.h"
#include "Matrix3f.h"

int main()
{
    /* Vector2f vec1(3.0f, 5.0f);
    Vector2f vec2(2.3f, 3.6f);

    std::cout << "vec1: "<< vec1 << std::endl;
    std::cout << "vec2: " << vec2 << std::endl;

    Vector2f op1 = vec1 + vec2;
    Vector2f op2 = vec1 - vec2;
    Vector2f op3 = 2.2f * vec1;
    Vector2f op4 = vec2 * 2.2f;
    Vector2f op5 = vec2 * vec1;
    Vector2f op6 = vec1 / 3.5f;

    float len = length(vec1);
    Vector2f norm = normalize(vec1);
    float sc = dotProduct(vec1, vec2);
    Vector2f perV = perp(vec1);

    Point2f point1(1.5f, 2.6f);
    Point2f point2(1.9f, 2.2f);


    Vector2f op7  = point1 - point2;
    Point2f op8  = point1 + vec1;
    Point2f op9  = point1 - vec1;

    float dist = distance(point1, point2);


    std::cout << "Operator+: " << op1 << std::endl;
    std::cout << "Operator-: " << op2 << std::endl;
    std::cout << "Operator*: " << op3 << std::endl;
    std::cout << "Operator*: " << op4 << std::endl;
    std::cout << "Operator*: " << op5 << std::endl;
    std::cout << "Operator/: " << op6 << std::endl;
    std::cout << "Length: "<< len << std::endl;
    std::cout << "Normalize: " << norm << std::endl;
    std::cout << "Scale: " << sc << std::endl;
    std::cout << "Perp: " << perV << std::endl;

    std::cout << "Operator-: " << op7 << std::endl;
    std::cout << "Operator+: " << op8 << std::endl;
    std::cout << "Operator-: " << op9 << std::endl;
    std::cout << "Distance-: " << dist << std::endl;*/


    Vector3f vec1(3.0f, 5.0f, 4.7f);
    Vector3f vec2(2.3f, 3.6f, 5.2f);

    std::cout << "vec1: "<< vec1 << std::endl;
    std::cout << "vec2: " << vec2 << std::endl;

    Vector3f op1 = vec1 + vec2;
    Vector3f op2 = vec1 - vec2;
    Vector3f op3 = 2.2f * vec1;
    Vector3f op4 = vec2 * 2.2f;
    Vector3f op5 = vec2 * vec1;
    Vector3f op6 = vec1 / 3.5f;


    Vector3f norm = normalize(vec1);
    Vector3f crossPr = crossProduct(vec1, vec2);

    float len = length(vec1);
    float sc = dotProduct(vec1, vec2);

    Point3f point1(1.5f, 2.6f, 3.6f);
    Point3f point2(1.9f, 2.2f, 0.5f);


    Vector3f op7  = point1 - point2;
    Point3f op8  = point1 + vec1;
    Point3f op9  = point1 - vec1;

    float dist = distance(point1, point2);

    std::cout << "Operator+: " << op1 << std::endl;
    std::cout << "Operator-: " << op2 << std::endl;
    std::cout << "Operator*: " << op3 << std::endl;
    std::cout << "Operator*: " << op4 << std::endl;
    std::cout << "Operator*: " << op5 << std::endl;
    std::cout << "Operator/: " << op6 << std::endl;
    std::cout << "Length: "<< len << std::endl;
    std::cout << "Normalize: " << norm << std::endl;
    std::cout << "DotProduct: " << sc << std::endl;
    std::cout << "CrossProduct: " << crossPr << std::endl;

    std::cout << "Operator-: " << op7 << std::endl;
    std::cout << "Operator+: " << op8 << std::endl;
    std::cout << "Operator-: " << op9 << std::endl;
    std::cout << "Distance-: " << dist << std::endl;


    //Matrix2f matrix1(2.0f, 3.0f, 4.0f, 5.0f);
   /* Matrix2f matrix2(6.0f, 7.0f, 8.0f, 9.0f);

    std::cout << "Matrix1:"<< "\n";
    std::cout<< matrix1 << std::endl;
    std::cout << "Matrix2:" <<"\n";
    std::cout<< matrix2 << std::endl;

    Matrix2f op1 = matrix1 + matrix2;
    Matrix2f op2 = matrix1 - matrix2;
    Matrix2f op3 = matrix1 * matrix2;
    Matrix2f op4 = matrix2 * 3;
    Matrix2f op5 = 4 * matrix1;

    float det = determinant(matrix1);
    Matrix2f tran = transpose(matrix1);


    std::cout << "Operator+:" << "\n";
    std::cout << op1 << std::endl;
    std::cout << "Operator-:" << "\n";
    std::cout << op2 << std::endl;
    std::cout << "Operator*:" << "\n";
    std::cout << op3 << std::endl;
    std::cout << "Operator*:" << "\n";
    std::cout << op4 << std::endl;
    std::cout << "Operator*:" << "\n";
    std::cout << op5 << std::endl;
    std::cout << "Determinant:" << "\n";
    std::cout << det << std::endl;
    std::cout << "Transpose:" << "\n";
    std::cout << tran << std::endl;
    std::cout << "Identity:" << "\n";
    std::cout << Matrix2f::identity << std::endl;
*/
    Matrix2f matrix(2.0f, 3.0f, 4.0f, 5.0f);
    Matrix2f invMatrix;
    bool inverted = inverse(matrix, &invMatrix);
    if (inverted)
    {
       std::cout << "Inverted:\n";
       std::cout << invMatrix;
    }
    else
       std::cout << "No inverted\n";

    //Matrix3f matrix1(2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f);
    /*Matrix3f matrix2(11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f);

    std::cout << "Matrix1:"<< "\n";
    std::cout<< matrix1 << std::endl;
    std::cout << "Matrix2:" <<"\n";
    std::cout<< matrix2 << std::endl;

    Matrix3f op1 = matrix1 + matrix2;
    Matrix3f op2 = matrix1 - matrix2;
    Matrix3f op3 = matrix1 * matrix2;
    Matrix3f op4 = matrix2 * 2;
    Matrix3f op5 = 2 * matrix1;

    float det = determinant(matrix1);
    Matrix3f tran = transpose(matrix1);


    std::cout << "Operator+:" << "\n";
    std::cout << op1 << std::endl;
    std::cout << "Operator-:" << "\n";
    std::cout << op2 << std::endl;
    std::cout << "Operator*:" << "\n";
    std::cout << op3 << std::endl;
    std::cout << "Operator*:" << "\n";
    std::cout << op4 << std::endl;
    std::cout << "Operator*:" << "\n";
    std::cout << op5 << std::endl;
    std::cout << "Determinant:" << "\n";
    std::cout << det << std::endl;
    std::cout << "Transpose:" << "\n";
    std::cout << tran << std::endl;
    std::cout << "Identity:" << "\n";
    std::cout << Matrix3f::identity << std::endl;

    std::cout << "Matrix1:"<< "\n";
    std::cout << matrix1 << std::endl;

    std::cout << "Matrix1 * Identity:" << "\n";
    std::cout << (matrix1 * Matrix3f::identity) << std::endl;

    std::cout << "Identity * Matrix1:" << "\n";
    std::cout << (Matrix3f::identity * matrix1) << std::endl;*/

    Matrix3f matrix3f = Matrix3f::identity;
    Matrix3f invMatrix3f;
    bool inverted3f = inverse(matrix3f, &invMatrix3f);
    if (inverted3f)
    {
       std::cout << "Inverted:\n";
       std::cout << invMatrix3f;
    }
    else
       std::cout << "No inverted\n";
    return 0;
}
