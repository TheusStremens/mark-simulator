#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.hpp"
#include "Intersection.hpp"
#include "Vehicle.hpp"

Intersection::Intersection()
{
  _type = ObjectType::objectIntersection;
}

void
Intersection::setBoundingBoxSize(const cv::Size &bb_size)
{
  cv::Point top_left = cv::Point(_pos_x - (bb_size.width / 2.0), _pos_y - (bb_size.height / 2.0));
  _bounding_box = cv::Rect(top_left, bb_size);
}

bool
Intersection::isInside(const int &pos_x, const int &pos_y)
{
  bool x_check = (_bounding_box.x < pos_x && pos_x < _bounding_box.x + _bounding_box.width);
  bool y_check = (_bounding_box.y < pos_y && pos_y < _bounding_box.y +_bounding_box.height);
  return x_check && y_check;
}

std::vector<std::shared_ptr<Street>> Intersection::queryStreets(std::shared_ptr<Street> incoming)
{
  // Store all outgoing streets in a vector ...
  std::vector<std::shared_ptr<Street>> outgoings;
  for (auto it : _streets)
  {
    if (incoming->getID() != it->getID()) // Except the street making the inquiry.
      outgoings.push_back(it);
  }

  return outgoings;
}

void Intersection::simulate()
{
  // Launch vehicle queue processing in a thread.
  _threads.emplace_back(std::thread(&Intersection::processVehicleQueue, this));
}

void Intersection::processVehicleQueue()
{
  // Continuously process the vehicle queue.
  while (true)
  {
    // Sleep at every iteration to reduce CPU usage.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}
