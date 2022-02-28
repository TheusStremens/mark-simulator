#include <iostream>
#include <thread>
#include "Graphics.hpp"
#include "Vehicle.hpp"
#include "Street.hpp"
#include "Intersection.hpp"


void
addLanesToWideVerticalStreet(std::shared_ptr<Street> &street)
{
  Lane lane_1(Direction::up);
  lane_1.setHorizontalStreetOffset(kFirstUpLaneVerticalStreet);
  street->addLane(lane_1);
  Lane lane_2(Direction::up);
  lane_2.setHorizontalStreetOffset(kSecondUpLaneVerticalStreet);
  street->addLane(lane_2);

  Lane lane_3(Direction::down);
  lane_3.setHorizontalStreetOffset(kFirstDownLaneVerticalStreet);
  street->addLane(lane_3);
  Lane lane_4(Direction::down);
  lane_4.setHorizontalStreetOffset(kSecondDownLaneVerticalStreet);
  street->addLane(lane_4);
}

void
addLanesToSmallVerticalStreet(std::shared_ptr<Street> &street)
{
  Lane lane_1(Direction::up);
  lane_1.setHorizontalStreetOffset(kFirstUpLaneVerticalStreet);
  street->addLane(lane_1);
  Lane lane_2(Direction::down);
  lane_2.setHorizontalStreetOffset(kFirstDownLaneVerticalStreet);
  street->addLane(lane_2);
}

void
addLanesToWideHorizontalStreet(std::shared_ptr<Street> &street)
{
  Lane lane_1(Direction::left);
  lane_1.setVerticalStreetOffset(kFirstLeftLaneHorizontalStreet);
  street->addLane(lane_1);
  Lane lane_2(Direction::left);
  lane_2.setVerticalStreetOffset(kSecondLeftLaneHorizontalStreet);
  street->addLane(lane_2);

  Lane lane_3(Direction::right);
  lane_3.setVerticalStreetOffset(kFirstRightLaneHorizontalStreet);
  street->addLane(lane_3);
  Lane lane_4(Direction::right);
  lane_4.setVerticalStreetOffset(kSecondRightLaneHorizontalStreet);
  street->addLane(lane_4);
}

void
addLanesToSmallHorizontalStreet(std::shared_ptr<Street> &street)
{
  Lane lane_1(Direction::left);
  lane_1.setVerticalStreetOffset(kFirstLeftLaneHorizontalStreet);
  street->addLane(lane_1);
  Lane lane_2(Direction::right);
  lane_2.setVerticalStreetOffset(kFirstRightLaneHorizontalStreet);
  street->addLane(lane_2);
}

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
  intersections.at(0)->setBoundingBoxSize(kHorizontalWideSquare);
  intersections.at(1)->setPosition(501, 502);
  intersections.at(1)->setBoundingBoxSize(kBigSquare);
  intersections.at(2)->setPosition(501, 242);
  intersections.at(2)->setBoundingBoxSize(kHorizontalWideSquare);
  // Left part from bottom to up.
  intersections.at(3)->setPosition(241, 781);
  intersections.at(3)->setBoundingBoxSize(kSmallSquare);
  intersections.at(4)->setPosition(241, 502);
  intersections.at(4)->setBoundingBoxSize(kVerticalWideSquare);
  intersections.at(5)->setPosition(241, 242);
  intersections.at(5)->setBoundingBoxSize(kSmallSquare);
  // Right part from bottom to up.
  intersections.at(6)->setPosition(771, 781);
  intersections.at(6)->setBoundingBoxSize(kSmallSquare);
  intersections.at(7)->setPosition(771, 502);
  intersections.at(7)->setBoundingBoxSize(kVerticalWideSquare);
  intersections.at(8)->setPosition(771, 242);
  intersections.at(8)->setBoundingBoxSize(kSmallSquare);

  // Central vertical streets from bottom to up.
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical, true));
  streets.at(0)->setIntersectionA(intersections.at(0));
  streets.at(0)->setIntersectionB(intersections.at(2));
  addLanesToWideVerticalStreet(streets.at(0));
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical));
  streets.at(1)->setIntersectionA(intersections.at(0));
  streets.at(1)->setIntersectionB(intersections.at(1));
  addLanesToWideVerticalStreet(streets.at(1));
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical));
  streets.at(2)->setIntersectionA(intersections.at(1));
  streets.at(2)->setIntersectionB(intersections.at(2));
  addLanesToWideVerticalStreet(streets.at(2));
  // Left vertical streets from bottom to up.
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical, true));
  streets.at(3)->setIntersectionA(intersections.at(3));
  streets.at(3)->setIntersectionB(intersections.at(5));
  addLanesToSmallVerticalStreet(streets.at(3));
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical));
  streets.at(4)->setIntersectionA(intersections.at(3));
  streets.at(4)->setIntersectionB(intersections.at(4));
  addLanesToSmallVerticalStreet(streets.at(4));
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical));
  streets.at(5)->setIntersectionA(intersections.at(4));
  streets.at(5)->setIntersectionB(intersections.at(5));
  addLanesToSmallVerticalStreet(streets.at(5));
  // Right vertical streets from bottom to up.
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical, true));
  streets.at(6)->setIntersectionA(intersections.at(6));
  streets.at(6)->setIntersectionB(intersections.at(8));
  addLanesToSmallVerticalStreet(streets.at(6));
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical));
  streets.at(7)->setIntersectionA(intersections.at(6));
  streets.at(7)->setIntersectionB(intersections.at(7));
  addLanesToSmallVerticalStreet(streets.at(7));
  streets.push_back(std::make_shared<Street>(StreetOrientation::vertical));
  streets.at(8)->setIntersectionA(intersections.at(7));
  streets.at(8)->setIntersectionB(intersections.at(8));
  addLanesToSmallVerticalStreet(streets.at(8));
  // Central horizontal streets from left to right.
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal, true));
  streets.at(9)->setIntersectionA(intersections.at(4));
  streets.at(9)->setIntersectionB(intersections.at(7));
  addLanesToWideHorizontalStreet(streets.at(9));
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal));
  streets.at(10)->setIntersectionA(intersections.at(4));
  streets.at(10)->setIntersectionB(intersections.at(1));
  addLanesToWideHorizontalStreet(streets.at(10));
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal));
  streets.at(11)->setIntersectionA(intersections.at(1));
  streets.at(11)->setIntersectionB(intersections.at(7));
  addLanesToWideHorizontalStreet(streets.at(11));
  // Bottom horizontal streets from left to right.
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal, true));
  streets.at(12)->setIntersectionA(intersections.at(3));
  streets.at(12)->setIntersectionB(intersections.at(6));
  addLanesToSmallHorizontalStreet(streets.at(12));
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal));
  streets.at(13)->setIntersectionA(intersections.at(3));
  streets.at(13)->setIntersectionB(intersections.at(0));
  addLanesToSmallHorizontalStreet(streets.at(13));
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal));
  streets.at(14)->setIntersectionA(intersections.at(0));
  streets.at(14)->setIntersectionB(intersections.at(6));
  addLanesToSmallHorizontalStreet(streets.at(14));
  // Top horizontal streets from left to right.
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal, true));
  streets.at(15)->setIntersectionA(intersections.at(5));
  streets.at(15)->setIntersectionB(intersections.at(8));
  addLanesToSmallHorizontalStreet(streets.at(15));
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal));
  streets.at(16)->setIntersectionA(intersections.at(5));
  streets.at(16)->setIntersectionB(intersections.at(2));
  addLanesToSmallHorizontalStreet(streets.at(16));
  streets.push_back(std::make_shared<Street>(StreetOrientation::horizontal));
  streets.at(17)->setIntersectionA(intersections.at(2));
  streets.at(17)->setIntersectionB(intersections.at(8));
  addLanesToSmallHorizontalStreet(streets.at(17));

  // Add vehicles to streets.
  for (size_t nv = 0; nv < 1; nv++)
  {
    vehicles.push_back(std::make_shared<Vehicle>());
    vehicles.at(nv)->setCurrentStreet(streets.at(nv % 9));
    vehicles.at(nv)->setCurrentDestination(intersections.at(nv % 9));
    vehicles.at(nv)->pickLane();
    vehicles.at(nv)->chooseStartingPosition();
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
