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
    intersections.push_back(std::make_shared<Intersection>());

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

  // Central vertical streets from bottom to up.
  streets.push_back(std::make_shared<Street>());
  streets.at(0)->setInIntersection(intersections.at(0));
  streets.at(0)->setOutIntersection(intersections.at(2));
  streets.push_back(std::make_shared<Street>());
  streets.at(1)->setInIntersection(intersections.at(0));
  streets.at(1)->setOutIntersection(intersections.at(1));
  streets.push_back(std::make_shared<Street>());
  streets.at(2)->setInIntersection(intersections.at(1));
  streets.at(2)->setOutIntersection(intersections.at(2));
  // Left vertical streets from bottom to up.
  streets.push_back(std::make_shared<Street>());
  streets.at(3)->setInIntersection(intersections.at(3));
  streets.at(3)->setOutIntersection(intersections.at(5));
  streets.push_back(std::make_shared<Street>());
  streets.at(4)->setInIntersection(intersections.at(3));
  streets.at(4)->setOutIntersection(intersections.at(4));
  streets.push_back(std::make_shared<Street>());
  streets.at(5)->setInIntersection(intersections.at(4));
  streets.at(5)->setOutIntersection(intersections.at(5));
  // Right vertical streets from bottom to up.
  streets.push_back(std::make_shared<Street>());
  streets.at(6)->setInIntersection(intersections.at(6));
  streets.at(6)->setOutIntersection(intersections.at(8));
  streets.push_back(std::make_shared<Street>());
  streets.at(7)->setInIntersection(intersections.at(6));
  streets.at(7)->setOutIntersection(intersections.at(7));
  streets.push_back(std::make_shared<Street>());
  streets.at(8)->setInIntersection(intersections.at(7));
  streets.at(8)->setOutIntersection(intersections.at(8));
  // Central horizontal streets from left to right.
  streets.push_back(std::make_shared<Street>());
  streets.at(9)->setInIntersection(intersections.at(4));
  streets.at(9)->setOutIntersection(intersections.at(7));
  streets.push_back(std::make_shared<Street>());
  streets.at(10)->setInIntersection(intersections.at(4));
  streets.at(10)->setOutIntersection(intersections.at(1));
  streets.push_back(std::make_shared<Street>());
  streets.at(11)->setInIntersection(intersections.at(1));
  streets.at(11)->setOutIntersection(intersections.at(7));
  // Bottom horizontal streets from left to right.
  streets.push_back(std::make_shared<Street>());
  streets.at(12)->setInIntersection(intersections.at(3));
  streets.at(12)->setOutIntersection(intersections.at(6));
  streets.push_back(std::make_shared<Street>());
  streets.at(13)->setInIntersection(intersections.at(3));
  streets.at(13)->setOutIntersection(intersections.at(0));
  streets.push_back(std::make_shared<Street>());
  streets.at(14)->setInIntersection(intersections.at(0));
  streets.at(14)->setOutIntersection(intersections.at(6));
  // Top horizontal streets from left to right.
  streets.push_back(std::make_shared<Street>());
  streets.at(15)->setInIntersection(intersections.at(5));
  streets.at(15)->setOutIntersection(intersections.at(8));
  streets.push_back(std::make_shared<Street>());
  streets.at(16)->setInIntersection(intersections.at(5));
  streets.at(16)->setOutIntersection(intersections.at(2));
  streets.push_back(std::make_shared<Street>());
  streets.at(17)->setInIntersection(intersections.at(2));
  streets.at(17)->setOutIntersection(intersections.at(8));

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
