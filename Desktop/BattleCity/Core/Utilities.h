#pragma once

#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QVector2D>

class GameUnit;

extern const float kEpsilon;

extern const QVector2D kUpDirection;
extern const QVector2D kDownDirection;
extern const QVector2D kRightDirection;
extern const QVector2D kLeftDirection;

extern const QPointF operator+ (const QPointF& point, const QSizeF& size);
extern const QPointF operator- (const QPointF& point, const QSizeF& size);
extern const QPointF operator* (const QPointF& point1, const QPointF& point2);

extern const QPointF operator+ (const QPointF& point, const QVector2D& vec);
extern const QPointF operator- (const QPointF& point, const QVector2D& vec);

extern const QPointF calcMapTileTopLeftLocation(int row, int col);
extern const QPointF calcMapTileBottomRightLocation(int row, int col);
extern const QPointF calcMapTileCenterLocation(int row, int col);
extern void calcMapTileRowAndCol(const QPointF& point, int& row, int& col);
extern const QVector2D truncate(const QVector2D& vec, float maxLength);

extern const QVector2D extractHeading(float rotationAngle);
extern float extractRotationAngle(const QVector2D& heading);

template <typename T>
bool belongsToRange(T minValue, T value, T maxValue)
{
    return (!(value < minValue) && !(maxValue < value));
}

extern float toRadians(float angleInDegrees);
extern float toDegrees(float angleInRadians);

extern bool equal(float left, float right, float epsilon = kEpsilon);
extern float sign(float number);
