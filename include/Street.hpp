#ifndef STREET_H
#define STREET_H

#include "TrafficObject.hpp"
#include "Lane.hpp"

// Forward declaration to avoid include cycle.
class Intersection;
class Lane;

enum StreetOrientation
{
  horizontal,
  vertical
};

class Street : public TrafficObject, public std::enable_shared_from_this<Street>
{
public:
  Street();
  Street(StreetOrientation orientation, bool is_loop_image_street = false);

  void setInIntersection(std::shared_ptr<Intersection> in);
  void setOutIntersection(std::shared_ptr<Intersection> out);
  inline std::shared_ptr<Intersection> getOutIntersection() { return _inter_out; }
  inline std::shared_ptr<Intersection> getInIntersection() { return _inter_in; }
  inline void setNumberLanes(uint lanes) { _number_lanes = lanes; }
  inline uint getNumberLanes() { return _number_lanes; }
  inline void addLane(const Lane &lane) { _lanes.push_back(lane); }

  /// Miscellaneous.
  std::shared_ptr<Street> get_shared_this() { return shared_from_this(); }

private:
  std::shared_ptr<Intersection> _inter_in, _inter_out; // intersections from which a vehicle can enter (one-way streets is always from 'in' to 'out')
  uint _number_lanes{1};
  /// If the street is horizontal or vertical.
  StreetOrientation _orientation;
  /// List of lanes of the street.
  std::vector<Lane> _lanes;
  /// If the street cross the limits of the image and appears in the opposite side.
  bool _is_loop_image_street{false};
};

#endif
