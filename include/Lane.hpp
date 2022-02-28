#ifndef LANE_HPP
#define LANE_HPP

/**
 * @brief The direction that the vehicle should move.
 *
 */
enum Direction
{
  up,
  down,
  left,
  right
};

/**
 * @brief Those constants define in pixels some offsets of the lane with respect to the
 * intersection position. For example, the first left lane in a horizontal street is 10
 * pixels (in Y) before any of the intersections that the street connects. In the other
 * hand, the first right lane in a horizontal street is 10 pixels (in Y) later then the
 * intersection.
 *
 */
// First Horizontal.
const int kFirstLeftLaneHorizontalStreet = -10;
const int kFirstRightLaneHorizontalStreet = 10;
// First Vertical.
const int kFirstUpLaneVerticalStreet = 10;
const int kFirstDownLaneVerticalStreet = -10;
// Second Horizontal.
const int kSecondLeftLaneHorizontalStreet = -30;
const int kSecondRightLaneHorizontalStreet = 30;
// Second Vertical.
const int kSecondUpLaneVerticalStreet = 30;
const int kSecondDownLaneVerticalStreet = -30;

/**
 * @brief This class helps to set the way that the vehicles should drive and provides
 * some offsets to make alignments. The class is very simple and small with only inline
 * methods.
 *
 */
class Lane
{
public:
  // Constructor
  inline Lane(Direction direction) : _direction(direction) {}

  // * Inline small methods that are self explanatory * //
  inline void setHorizontalStreetOffset(int value) { _horizontal_street_offset = value; }
  inline void setVerticalStreetOffset(int value) { _vertical_street_offset = value; }
  inline int getHorizontalStreetOffset() { return _horizontal_street_offset; }
  inline int getVerticalStreetOffset() { return _vertical_street_offset; }
  inline Direction getDirection() { return _direction; }

private:
  /// Lane direction.
  Direction _direction;
  /// The horizontal offset in pixels with respect to the street.
  int _horizontal_street_offset{0};
  /// The vertical offset in pixels with respect to the street.
  int _vertical_street_offset{0};
};

#endif
