#include <iostream>
#include "Vehicle.hpp"
#include "Intersection.hpp"
#include "Street.hpp"

Street::Street()
{
  _type = ObjectType::objectStreet;
}

Street::Street(StreetOrientation orientation, bool is_loop_image_street)
  : _orientation(orientation),
    _is_loop_image_street(is_loop_image_street)
{
  _type = ObjectType::objectStreet;
}

void Street::setIntersectionA(std::shared_ptr<Intersection> intersection)
{
  _intersection_A = intersection;
  // Add this street to list of streets connected to the intersection.
  intersection->addStreet(get_shared_this());
}

void Street::setIntersectionB(std::shared_ptr<Intersection> intersection)
{
  _intersection_B = intersection;
  // Add this street to list of streets connected to the intersection.
  intersection->addStreet(get_shared_this());
}

std::vector<std::shared_ptr<Lane>>
Street::getLanesByDirection(Direction lane_direction)
{
  std::vector<std::shared_ptr<Lane>> result;
  for (auto lane : _lanes)
  {
    if (lane->getDirection() == lane_direction)
      result.push_back(lane);
  }
  return result;
}
