#ifndef LANE_HPP
#define LANE_HPP

enum Direction
{
  up,
  down,
  left,
  right
};

const int kFirstLeftLaneHorizontalStreet = -10;
const int kFirstRightLaneHorizontalStreet = 10;
const int kFirstUpLaneVerticalStreet = 10;
const int kFirstDownLaneVerticalStreet = -10;
const int kSecondLeftLaneHorizontalStreet = 30;
const int kSecondRightLaneHorizontalStreet = -30;
const int kSecondUpLaneVerticalStreet = 10;
const int kSecondDownLaneVerticalStreet = -10;

class Lane
{
public:
  inline Lane(Direction direction) : _direction(direction) {}
  inline void setHorizontalStreetOffset(int value) { _horizontal_street_offset = value; }
  inline void setVerticalStreetOffset(int value) { _vertical_street_offset = value; }

  inline int getHorizontalStreetOffset() { return _horizontal_street_offset; }
  inline int getVerticalStreetOffset() { return _vertical_street_offset; }

private:
  /// Lane direction.
  Direction _direction;
  /// The horizontal offset in pixels with respect to the street.
  int _horizontal_street_offset{0};
  /// The vertical offset in pixels with respect to the street.
  int _vertical_street_offset{0};
};

#endif
