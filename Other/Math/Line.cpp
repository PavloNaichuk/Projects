#include "Line.h"
#include "Utilities.h"

Line::Line(const Point2f& start, const Vector2f& dir)
    : mStart(start)
    , mDir(dir)
{}

Line::Line(const Point2f& start, const Point2f& end)
    : mStart(start)
    , mDir(start, end)
{
}

const Point2f calcPointAtParam(const Line& line, float param)
{
    return (line.mStart + param * line.mDir);
}

bool Overlaps(const Line& line1, const Line& line2, Point2f* pOverlapPoint)
{
    Vector2f perpDir2 = perp(line2.mDir);
    float denom = dotProduct(perpDir2, line1.mDir);

    if (!equals(denom, 0.0f))
    {
        Vector2f perpDir1 = perp(line1.mDir);
        Vector2f diff = line2.mStart - line1.mStart;

        float param1 = dotProduct(perpDir2, diff) / denom;
        float param2 = dotProduct(perpDir1, diff) / denom;

        if (belongsToRange(param1, 0.0f, 1.0f) && belongsToRange(param2, 0.0f, 1.0f))
        {
            if (pOverlapPoint != nullptr)
                *pOverlapPoint = calcPointAtParam(line1, param1);
            return true;
        }
    }
    return false;
}
