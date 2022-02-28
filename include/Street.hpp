#ifndef STREET_H
#define STREET_H

#include "TrafficObject.hpp"
#include "Lane.hpp"

// Forward declaration to avoid include cycle.
class Intersection;
class Lane;

/**
 * @brief The street direction helps to define the lane directions and take some
 * decisions.
 *
 */
enum StreetOrientation
{
  horizontal,
  vertical
};

/**
 * @brief The street class connects two intersections (A and B) and contains two or more
 * lanes. All the streets have at lest 2 lanes (one in each way). There are some special
 * streets that can loop over the image.
 *
 */
class Street : public TrafficObject, public std::enable_shared_from_this<Street>
{
public:
  /// Constructors.
  Street();
  Street(StreetOrientation orientation, bool is_loop_image_street = false);

  /**
   * @brief Set the Intersection A member and add a pointer to this street in the
   * intersection.
   *
   * @param intersection
   */
  void setIntersectionA(std::shared_ptr<Intersection> intersection);
  /**
   * @brief Set the Intersection B member and add a pointer to this street in the
   * intersection.
   *
   * @param intersection
   */
  void setIntersectionB(std::shared_ptr<Intersection> intersection);

  /**
   * @brief Return a list of lanes that have the same direction of the argument
   * lane_direction.
   *
   * @param lane_direction
   * @return std::vector<std::shared_ptr<Lane>>
   */
  std::vector<std::shared_ptr<Lane>> getLanesByDirection(Direction lane_direction);

  // * Inline small methods that are self explanatory * //
  inline std::shared_ptr<Intersection> getIntersectionA() { return _intersection_A; }
  inline std::shared_ptr<Intersection> getIntersectionB() { return _intersection_B; }
  inline void addLane(const Lane &lane) { _lanes.push_back(std::make_shared<Lane>(lane)); }
  inline bool isLoopStreet() { return _is_loop_image_street; }
  inline StreetOrientation getOrientation() { return _orientation; }

  /// Miscellaneous.
  std::shared_ptr<Street> get_shared_this() { return shared_from_this(); }

private:
  /// A street connects the intersection A to the intersection B.
  std::shared_ptr<Intersection> _intersection_A, _intersection_B;
  /// If the street is horizontal or vertical.
  StreetOrientation _orientation;
  /// List of lanes of the street.
  std::vector<std::shared_ptr<Lane>> _lanes;
  /// If the street cross the limits of the image and appears in the opposite side.
  bool _is_loop_image_street{false};
};

#endif
