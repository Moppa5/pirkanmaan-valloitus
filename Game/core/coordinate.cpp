#include "coordinate.h"

namespace Course {

Coordinate::Coordinate(int x, int y):
    m_x(x), m_y(y)
{

}

Coordinate::Coordinate(const Coordinate& original):
    m_x(original.x()), m_y(original.y())
{
}

Coordinate::Coordinate(const Coordinate& original,
                       Direction direction,
                       int steps):
    m_x(original.x()), m_y(original.y())
{
    travel(direction, steps);
}

int Coordinate::x() const
{
    return m_x;
}

int Coordinate::y() const
{
    return m_y;
}

void Coordinate::set_x(int x)
{
    m_x = x;
}

void Coordinate::set_y(int y)
{
    m_y = y;
}

void Coordinate::travel(Direction direction, int steps)
{
    int delta_x = 0;
    int delta_y = 0;

    translate_direction(direction, delta_x, delta_y, steps);

    m_x += delta_x;
    m_y += delta_y;
}

Coordinate Coordinate::neighbour_at(Direction direction, int steps) const
{
    int delta_x = 0;
    int delta_y = 0;

    translate_direction(direction, delta_x, delta_y, steps);

    Coordinate neighbour(m_x + delta_x, m_y + delta_y);

    return neighbour;
}

std::vector<Coordinate> Coordinate::neighbours(const int& radius) const
{
    std::vector<Coordinate> neighbour_vector;

    int x_lower = m_x - radius;
    int y_lower = m_y - radius;
    int x_upper = m_x + radius;
    int y_upper = m_y + radius;

    for( int x = x_lower; x <= x_upper; ++x )
    {
        for( int y = y_lower; y <= y_upper; ++y )
        {
            if( x == m_x and y == m_y )
            {
                continue;
            }
            neighbour_vector.push_back(Coordinate(x,y));
        }
    }

    return neighbour_vector;
}

Coordinate Coordinate::operator+(const Coordinate& other) const
{
    return Coordinate(m_x + other.x(), m_y + other.y());
}

Coordinate &Coordinate::operator+=(const Coordinate& other)
{
    m_x += other.x();
    m_y += other.y();

    return *this;
}

Coordinate Coordinate::operator-(const Coordinate& other) const
{
    return Coordinate(m_x - other.x(), m_y - other.y());
}

Coordinate &Coordinate::operator-=(const Coordinate& other)
{
    m_x -= other.x();
    m_y -= other.y();

    return *this;
}

Coordinate &Coordinate::operator=(const Coordinate& other)
{
    m_x = other.x();
    m_y = other.y();

    return *this;
}

bool Coordinate::operator==(const Coordinate& other) const
{
    return m_x == other.x() and m_y == other.y();
}

bool Coordinate::operator!=(const Coordinate& other) const
{
    return m_x != other.x() or m_y != other.y();
}

bool Coordinate::operator<(const Coordinate& other) const
{
    if(m_x < other.x())
    {
        return true;
    }
    if(m_x == other.x())
    {
        return m_y < other.y();
    }

    return false;
}

bool Coordinate::operator>(const Coordinate& other) const
{
    if(m_x > other.x())
    {
        return true;
    }
    if(m_x == other.x())
    {
        return m_y > other.y();
    }

    return false;
}

bool Coordinate::operator<=(const Coordinate& other) const
{
    return *this < other or *this == other;
}

bool Coordinate::operator>=(const Coordinate& other) const
{
    return *this > other or *this == other;
}

#ifdef QT_CORE_LIB
QPoint Coordinate::asQpoint() const
{
    return QPoint(m_x, m_y);
}
#endif

void Coordinate::translate_direction(Direction direction,
                                     int &delta_x,
                                     int &delta_y,
                                     const int& steps) const
{
    switch (direction) {
    case N:
        delta_x = 0;
        delta_y = 1;
        break;
    case NE:
        delta_x = 1;
        delta_y = 1;
        break;
    case E:
        delta_x = 1;
        delta_y = 0;
        break;
    case SE:
        delta_x = 1;
        delta_y = -1;
        break;
    case S:
        delta_x = 0;
        delta_y = -1;
        break;
    case SW:
        delta_x = -1;
        delta_y = -1;
        break;
    case W:
        delta_x = -1;
        delta_y = 0;
        break;
    case NW:
        delta_x = -1;
        delta_y = 1;
        break;
    case END:
        delta_x = 0;
        delta_y = 0;
        break;
    }

    delta_x = delta_x * steps;
    delta_y = delta_y * steps;
}



} // namespace BoardGameBase
