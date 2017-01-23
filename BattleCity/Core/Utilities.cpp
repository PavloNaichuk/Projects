#include "Utilities.h"
#include "Config.h"
#include "GameUnit.h"
#include <algorithm>

const float kEpsilon = 0.0000001f;

const QVector2D kUpDirection(0.0f, -1.0f);
const QVector2D kDownDirection(0.0f, 1.0f);
const QVector2D kRightDirection(1.0f, 0.0f);
const QVector2D kLeftDirection(-1.0f, 0.0f);

const QPointF operator+ (const QPointF& point, const QSizeF& size)
{
    return QPointF(point.x() + size.width(), point.y() + size.height());
}

const QPointF operator- (const QPointF& point, const QSizeF& size)
{
    return QPointF(point.x() - size.width(), point.y() - size.height());
}

const QPointF operator* (const QPointF& point1, const QPointF& point2)
{
    return QPointF(point1.x() * point2.x(), point1.y() * point2.y());
}

const QPointF operator+ (const QPointF& point, const QVector2D& vec)
{
    return QPointF(point.x() + vec.x(), point.y() + vec.y());
}

const QPointF operator- (const QPointF& point, const QVector2D& vec)
{
    return QPointF(point.x() - vec.x(), point.y() - vec.y());
}

const QPointF calcMapTileTopLeftLocation(int row, int col)
{
    return QPointF(col * Config::TILE_SCREEN_SIZE, row * Config::TILE_SCREEN_SIZE);
}

const QPointF calcMapTileBottomRightLocation(int row, int col)
{
    return (calcMapTileTopLeftLocation(row, col) + QSizeF(Config::TILE_SCREEN_SIZE - 1, Config::TILE_SCREEN_SIZE - 1));
}

const QPointF calcMapTileCenterLocation(int row, int col)
{
    return (calcMapTileTopLeftLocation(row, col) + QSizeF(Config::TILE_SCREEN_HALF_SIZE, Config::TILE_SCREEN_HALF_SIZE));
}

void calcMapTileRowAndCol(const QPointF& point, int& row, int& col)
{
    Q_ASSERT(belongsToRange(0.0, point.y(), double(Config::SCREEN_HEIGHT - 1)));
    Q_ASSERT(belongsToRange(0.0, point.x(), double(Config::SCREEN_WIDTH - 1)));

    row = point.y() / Config::TILE_SCREEN_SIZE;
    col = point.x() / Config::TILE_SCREEN_SIZE;
}

const QVector2D truncate(const QVector2D& vec, float maxLength)
{
    float length = vec.length();
    return (length > maxLength) ? (vec * (maxLength / length)) : vec;
}

const QVector2D extractHeading(float rotationAngle)
{
    float angleInRadians = toRadians(rotationAngle);
    return QVector2D(std::sin(angleInRadians), -std::cos(angleInRadians));
}

float extractRotationAngle(const QVector2D& heading)
{
    if (heading == kRightDirection)
        return 90.0f;
    if (heading == kDownDirection)
        return 180.0f;
    if (heading == kLeftDirection)
        return 270.0f;

    Q_ASSERT(heading == kUpDirection);
    return 0.0f;
}

float toRadians(float angleInDegrees)
{
    static const float PI = 3.141592654f;
    static const float degreesToRadians = PI / 180.0f;

    return degreesToRadians * angleInDegrees;
}

float toDegrees(float angleInRadians)
{
    static const float PI = 3.141592654f;
    static const float radiansToDegrees = 180.0f / PI;

    return radiansToDegrees * angleInRadians;
}

bool equal(float left, float right, float epsilon)
{
    return (std::abs(left - right) < epsilon);
}

float sign(float number)
{
    if (number > 0.0f)
        return 1.0f;
    if (number < 0.0f)
        return -1.0f;
    return 0.0f;
}
