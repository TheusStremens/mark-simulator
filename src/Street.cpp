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

void Street::setInIntersection(std::shared_ptr<Intersection> in)
{
  _inter_in = in;
  // Add this street to list of streets connected to the intersection.
  in->addStreet(get_shared_this());
}

void Street::setOutIntersection(std::shared_ptr<Intersection> out)
{
  _inter_out = out;
  // Add this street to list of streets connected to the intersection.
  out->addStreet(get_shared_this());
}
