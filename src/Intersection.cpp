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
  _is_blocked = false;
}

void Intersection::addStreet(std::shared_ptr<Street> street)
{
  _streets.push_back(street);
}

void
Intersection::setBoundingBoxSize(const cv::Size &bb_size)
{
  cv::Point top_left = cv::Point(_posX - bb_size.width, _posY - bb_size.height);
  _bounding_box = cv::Rect(top_left, bb_size);
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

// Adds a new vehicle to the queue and returns once the vehicle is allowed to enter.
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

void Intersection::vehicleHasLeft()
{
  // Unblock queue processing.
  this->setIsBlocked(false);
}

void Intersection::simulate()
{
  // Launch vehicle queue processing in a thread.
  threads.emplace_back(std::thread(&Intersection::processVehicleQueue, this));
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
