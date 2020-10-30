#ifndef COORDINATE_H
#define COORDINATE_H

#ifdef QT_CORE_LIB
    #include <qpoint.h>
#endif

#include <vector>

namespace Course {

/**
 * @brief Direction is an enumeration to describe direction via
 * cardinal and intercardinal compass directions.
 */
enum Direction{N=0,NE=1,E=2,SE=3,S=4,SW=5,W=6,NW=7, END=8};


class Coordinate
{
public:

    Coordinate() = delete;

    /**
     * @brief Constructor for x, y coordinates
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    Coordinate(int x, int y);

    /**
     * @brief Copy-constructor
     * @param original Reference to a Coordinate that is being copied
     */
    Coordinate(const Coordinate& original);

    /**
     * @brief Copy-constructor where the new Coordinate is moved.
     * @param original Reference to Coordinate that is being copied
     * @param direction What direction is the movement applied
     * @param steps How many steps towards that direction is taken, default = 1
     * @post Exception guarantee: No-throw
     */
    Coordinate(const Coordinate& original, Direction direction, int steps=1);

    /**
     * @brief Default destructor
     * @post Exception guarantee: Strong
     */
    ~Coordinate() = default;

    /**
     * @brief x Return's the x-value
     * @return x-coordinate
     * @post Exception guarantee: No-throw
     */
    int x() const;

    /**
     * @brief y Return's the y-value
     * @return y-coordinate
     * @post Exception guarantee: No-throw
     */
    int y() const;

    /**
     * @brief set_x Sets the x-value
     * @param x New x-value
     * @post Exception guarantee: No-throw
     */
    void set_x(int x);
    /**
     * @brief set_y Sets the y-value
     * @param y New y-value
     * @post Exception guarantee: No-throw
     */
    void set_y(int y);

    /**
     * @brief travel Increments the coordinate values to given direction.
     * Optionally can give how many increments are done.
     * @param direction Direction in x,y -coordinate system
     * @param steps How many increments of the values are done.
     * @post Exception guarantee: No-throw
     */
    void travel(Direction direction, int steps=1);

    /**
     * @brief neighbour_at Returns a new Coordinate object instead of changing
     * the current one
     * @param direction Direction in x,y -coordinate system
     * @param steps How many increments of the values are done.
     * @return Coordinate object that was created
     * @post Exception guarantee: No-throw
     */
    Coordinate neighbour_at(Direction direction, int steps=1) const;
    /**
     * @brief neighbours Returns a Coordinate for each direction.
     * The created vector has the Coordinates in order of
     * the default direction values
     * @return vector of Coordinates in the order of directions.
     * @post Exception guarantee: Strong
     */
    std::vector<Coordinate> neighbours(const int &radius = 1) const;

    /**
     * @brief operator + Sums two coordinates' x and y values together and
     * returns a new Coordinate with the summed values.
     * @param other The other Coordinate
     * @return Coordinate with x = x(this) + x(other) and y = y(this) + y(other)
     * @post Exception guarantee: No-throw
     */
    Coordinate operator+(const Coordinate& other) const;
    /**
     * @brief operator += Adds other coordinate's x and y values to this.
     * @param other The other coordinate
     * @return Reference to self.
     * @post Exception guarantee: No-throw
     */
    Coordinate& operator+=(const Coordinate& other);
    /**
     * @brief operator - Substracts right side coordinate's x and y values
     * from this x and y values.
     * Returns a new Coordinate with new x and y values
     * @param other The other coordinate
     * @return Coordinate with x = x(this) - x(other) and y = y(this) - y(other)
     * @post Exception guarantee: No-throw
     */
    Coordinate operator-(const Coordinate& other) const;
    /**
     * @brief operator -= Substracts other coordinate's x and y values from this
     * @param other The other coordinate
     * @return Reference to self.
     * @post Exception guarantee: No-throw
     */
    Coordinate& operator-=(const Coordinate& other);

    /**
     * @brief operator = Assigns other Coordinate's x and y values to this.
     * @param other The other Coordinate
     * @return Reference to self.
     * @post Exception guarantee: No-throw
     */
    Coordinate& operator=(const Coordinate& other);

    /**
     * @brief operator == Checks if a coordinate has same x and y values as this
     * @param other The other Coordinate
     * @return
     * True - if the coordinates have same values.
     * @post Exception guarantee: No-throw
     */
    bool operator==(const Coordinate& other) const;

    /**
     * @brief operator == Checks if a coordinate doesnt' have same
     * x and y values as this
     * @param other The other Coordinate
     * @return
     * True - if the coordinates don't have same values.
     * @post Exception guarantee: No-throw
     */
    bool operator!=(const Coordinate& other) const;

    /**
     * @brief operator < Checks if this has lower value than other.
     * Decision is based on x-value.
     * If the x-values are equal, decision is based on y-value
     * @param other The other Coordinate
     * @return
     * True - (x(this) < x(other)) or (x(this) == x(other) + y(this) < y(other))
     * @post Exception guarantee: No-throw
     */
    bool operator<(const Coordinate& other) const;

    /**
     * @brief operator < Checks if this has higher value than other.
     * Decision is based on x-value.
     * If the x-values are equal, decision is based on y-value
     * @param other The other Coordinate
     * @return
     * True - (x(this) > x(other)) or (x(this) == x(other) + y(this) > y(other))
     * @post Exception guarantee: No-throw
     */
    bool operator>(const Coordinate& other) const;

    /**
     * @brief operator <= logical-OR with == and < operators
     * @param other The other Coordinate
     * @post Exception guarantee: No-throw
     */
    bool operator<=(const Coordinate& other) const;

    /**
     * @brief operator <= logical-OR with == and > operators
     * @param other The other Coordinate
     * @post Exception guarantee: No-throw
     */
    bool operator>=(const Coordinate& other) const;

#ifdef QT_CORE_LIB
    QPoint asQpoint() const;
#endif

private:

    int m_x;
    int m_y;

    void translate_direction(Direction direction,
                             int& delta_x,
                             int& delta_y,
                             const int& steps) const;
};
}
#endif // COORDINATE_H
