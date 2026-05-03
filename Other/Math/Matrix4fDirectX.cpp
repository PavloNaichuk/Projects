#include "Matrix4fDirectX.h"
#include "Point3f.h"
#include "Vector3f.h"
#include <cmath>

Matrix4f CreateLookAtMatrix(const Point3f& worldCameraPos, const Point3f& worldCameraFocus, const Vector3f& worldUp)
{
    Vector3f basisY = normalize(worldUp);
    Vector3f basisZ = normalize(worldCameraFocus - worldCameraPos);
    Vector3f basisX = normalize(crossProduct(basisY, basisZ));

    Matrix4f worldMatrix(basisX.mX,          basisX.mY,           basisX.mZ,          0.0f,
                         basisY.mX,          basisY.mY,           basisY.mZ,          0.0f,
                         basisZ.mX,          basisZ.mY,           basisZ.mZ,          0.0f,
                         worldCameraPos.mX,  worldCameraPos.mY,   worldCameraPos.mZ,  1.0f);

    Matrix4f worldToCameraMatrix;
    inverse(worldMatrix, &worldToCameraMatrix);
    return worldToCameraMatrix;
}
