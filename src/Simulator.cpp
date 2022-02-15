#include <iostream>
#include <thread>
#include "Graphics.hpp"
#include "Vehicle.hpp"
#include "Street.hpp"
#include "Intersection.hpp"


void
createTrafficObjects(std::vector<std::shared_ptr<Street>> &streets,
                     std::vector<std::shared_ptr<Intersection>> &intersections,
                     std::vector<std::shared_ptr<Vehicle>> &vehicles)
{
  for (size_t i = 0; i < 9; i++)
  {
    intersections.push_back(std::make_shared<Intersection>());
  }

  // Position intersections in pixel coordinates.
  // Middle part from bottom to up.
  intersections.at(0)->setPosition(501, 781);
  intersections.at(1)->setPosition(501, 502);
  intersections.at(2)->setPosition(501, 242);
  // Left part from bottom to up.
  intersections.at(3)->setPosition(241, 781);
  intersections.at(4)->setPosition(241, 502);
  intersections.at(5)->setPosition(241, 242);
  // Right part from bottom to up.
  intersections.at(6)->setPosition(771, 781);
  intersections.at(7)->setPosition(771, 502);
  intersections.at(8)->setPosition(771, 242);

  // Create streets and connect with the intersections.
  int nStreets = 8;
  for (size_t ns = 0; ns < nStreets; ns++)
  {
    streets.push_back(std::make_shared<Street>());
    streets.at(ns)->setInIntersection(intersections.at(ns));
    streets.at(ns)->setOutIntersection(intersections.at(ns+1));
  }

  // Add vehicles to streets.
  for (size_t nv = 0; nv < 1; nv++)
  {
    vehicles.push_back(std::make_shared<Vehicle>());
    vehicles.at(nv)->setCurrentStreet(streets.at(nv));
    vehicles.at(nv)->setCurrentDestination(intersections.at(nv));
  }
}

int main(int argc, char *argv[])
{
  // Create and connect intersections and streets.
  std::vector<std::shared_ptr<Street>> streets;
  std::vector<std::shared_ptr<Intersection>> intersections;
  std::vector<std::shared_ptr<Vehicle>> vehicles;
  createTrafficObjects(streets, intersections, vehicles);

  // simulate intersection
  std::for_each(intersections.begin(), intersections.end(), [](std::shared_ptr<Intersection> &i) {
    i->simulate();
  });

  // simulate vehicles
  std::for_each(vehicles.begin(), vehicles.end(), [](std::shared_ptr<Vehicle> &v) {
    v->simulate();
  });

  // Add all objects into common vector to be displayed.
  std::vector<std::shared_ptr<TrafficObject>> traffic_objects;

   std::for_each(vehicles.begin(), vehicles.end(), [&traffic_objects](std::shared_ptr<Vehicle> &vehicles) {
      std::shared_ptr<TrafficObject> traffic_object = std::dynamic_pointer_cast<TrafficObject>(vehicles);
      traffic_objects.push_back(traffic_object);
  });

  std::for_each(intersections.begin(), intersections.end(), [&traffic_objects](std::shared_ptr<Intersection> &intersection)
  {
    std::shared_ptr<TrafficObject> traffic_object = std::dynamic_pointer_cast<TrafficObject>(intersection);
    traffic_objects.push_back(traffic_object);
  });

  Graphics graphics;
  graphics.loadBackgroundImage();
  graphics.setTrafficObjects(traffic_objects);
  graphics.simulate();
  return 0;
}
