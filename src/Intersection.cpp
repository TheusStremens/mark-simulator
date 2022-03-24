#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.hpp"
#include "Intersection.hpp"
#include "Vehicle.hpp"

/// Implementation of class "WaitingVehicles" ///

void WaitingVehicles::pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise)
{
  std::lock_guard<std::mutex> lock(_mutex);

  _vehicles.push_back(vehicle);
  _promises.push_back(std::move(promise));
}

void WaitingVehicles::permitEntryToFirstInQueue()
{
  std::lock_guard<std::mutex> lock(_mutex);

  // Get entries from the front of both queues.
  auto first_promise = _promises.begin();
  auto first_vehicle = _vehicles.begin();

  // Fulfill promise and send signal back that permission to enter has been granted.
  first_promise->set_value();

  // Remove front elements from both queues.
  _vehicles.erase(first_vehicle);
  _promises.erase(first_promise);
}

/// Implementation of class "Intersection" ///

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

bool
Intersection::isCompletelyInside(const int &pos_x, const int &pos_y)
{
  bool x_check = (_bounding_box.x + 5.0 < pos_x && pos_x < _bounding_box.x + _bounding_box.width -5.0);
  bool y_check = (_bounding_box.y + 5.0 < pos_y && pos_y < _bounding_box.y +_bounding_box.height -5.0);
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

void Intersection::addVehicleToQueue(std::shared_ptr<Vehicle> vehicle)
{
  std::unique_lock<std::mutex> lck(_mtx);
  std::cout << "Intersection #" << _id << "::addVehicleToQueue: thread id = " << std::this_thread::get_id() << std::endl;
  lck.unlock();

  // Add new vehicle to the end of the waiting line.
  std::promise<void> proms_vehicle_allowed_to_enter;
  std::future<void> ftr_vehicle_allowed_to_enter = proms_vehicle_allowed_to_enter.get_future();
  _waiting_vehicles.pushBack(vehicle, std::move(proms_vehicle_allowed_to_enter));

  // Wait until the vehicle is allowed to enter.
  ftr_vehicle_allowed_to_enter.wait();
  lck.lock();
  std::cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " is granted entry." << std::endl;
  lck.unlock();
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

    // Only proceed when at least one vehicle is waiting in the queue.
    if (_waiting_vehicles.getSize() > 0 && !_is_blocked)
    {
        // Set intersection to "blocked" to prevent other vehicles from entering.
        this->setIsBlocked(true);

        // permit entry to first vehicle in the queue (FIFO)
        _waiting_vehicles.permitEntryToFirstInQueue();
    }
  }
}
