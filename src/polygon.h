#pragma once
#include <vector>
#include <cmath>

inline bool areEqual(double x, double y)
{
    const double epsilon = 1e-5;
    return std::abs(x - y) <= epsilon * std::abs(x);
}

struct Point {
    Point(double x, double y) : X{ x }, Y{ y }
    {}
    bool operator==(const Point& rhs) {
        return areEqual(rhs.X, X) && areEqual(rhs.Y, Y);
    }
    double X;
    double Y;
};

/*
  ������, �������� �� ������������� ��������.
  ����: ������ �� N ����� (x, y) ����������� �������������. ����� �������� �� ������� �������.
  �����: bool.
*/
bool isConvexPolygon(const std::vector<Point>& vertices) 
{
    if (vertices.size() < 3) return false;
    for (size_t i = 1; i < vertices.size(); i++)
    {
        auto a = vertices[i - 1];
        auto b = vertices[i];
        auto c = (i + 1 < vertices.size() ? vertices[i + 1] : vertices[0]);
        auto planarTriangle = (b.X - a.X)*(c.Y - a.Y) - (b.Y - a.Y)*(c.X - a.X);
        if (planarTriangle > 0)
        {
            //������� ������ ������� �������, ������ �� ��������
            return false;
        }
    }
    return true;
}

/*
    ������, ����������� �� ����� ��������������.
    ����: ������ �� N �����(x, y).������ N - 1 ����� ��������� �������������.����� �������� �� ������� �������.��������� �������� ����������� ������.
    ����� : bool.
*/
bool isPointInsidePolygon(const std::vector<Point>& points)
{
    if (points.size() < 4) return false;

    auto point = points.back();
    auto intersectCount = 0;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        auto a = points[i];
        auto b = (i < points.size() - 2 ? points[i + 1] : points[0]);

        if (point == a || point == b) //����� ��������� � ��������
        {
            return true;
        }
        if (areEqual(a.Y, b.Y) && areEqual(a.Y, point.Y)) //��� �������� ����� �������������� �������
        {
            if (a.X > point.X && b.X < point.X
                || a.X < point.X && b.X > point.X) //����� ������ ��������������� �������
            {
                return true;
            }
        }
        if (areEqual(a.Y, point.Y) && point.Y > b.Y) //��� �������� ����� �������
        {
            intersectCount++;
        }
        else if (areEqual(b.Y, point.Y) && point.Y > a.Y) //��� �������� ����� �������
        {
            intersectCount++;
        }
        else if (a.Y > point.Y && b.Y < point.Y
                 || b.Y > point.Y && a.Y < point.Y)
        {
            auto intersectX = a.X + (point.Y - a.Y) * (b.X - a.X) / (b.Y - a.Y);
            if (areEqual(point.X, intersectX)) //����� �� �������
            {
                return true;
            }
            else if (point.X < intersectX)
            {
                intersectCount++;
            }
        }
    }
    return intersectCount % 2 == 1;
}
