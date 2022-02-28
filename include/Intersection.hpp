#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include <opencv2/core.hpp>
#include "TrafficObject.hpp"

// Forward declarations to avoid include cycle.
class Street;
class Vehicle;

const cv::Size kBigSquare = cv::Size(80, 80);
const cv::Size kSmallSquare = cv::Size(40, 40);
const cv::Size kHorizontalWideSquare = cv::Size(80, 40);
const cv::Size kVerticalWideSquare = cv::Size(40, 80);

// Auxiliary class to queue and dequeue waiting vehicles in a thread-safe manner
class WaitingVehicles
{
public:
  inline int getSize()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    return _vehicles.size();
  };

  void pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise);
  void permitEntryToFirstInQueue();

private:
  /// List of all vehicles waiting to enter this intersection.
  std::vector<std::shared_ptr<Vehicle>> _vehicles;
  /// List of associated promises.
  std::vector<std::promise<void>> _promises;
  /// Mutex to avoid data race.
  std::mutex _mutex;
};

class Intersection : public TrafficObject
{
public:
  Intersection();

  inline void setIsBlocked(bool value) { _is_blocked = value; };

  void addVehicleToQueue(std::shared_ptr<Vehicle> vehicle);
  void addStreet(std::shared_ptr<Street> street);
  /// Return pointer to current list of all outgoing streets.
  std::vector<std::shared_ptr<Street>> queryStreets(std::shared_ptr<Street> incoming);
  void simulate();
  void vehicleHasLeft();
  bool trafficLightIsGreen();
  void setBoundingBoxSize(const cv::Size &bb_size);
  bool isInside(const int &pos_x, const int &pos_y);
  inline cv::Rect getBoundingBox() { return _bounding_box; }

private:
  void processVehicleQueue();

  /// List of all streets connected to this intersection.
  std::vector<std::shared_ptr<Street>> _streets;
  /// list of all vehicles and their associated promises waiting to enter the intersection.
  WaitingVehicles _waiting_vehicles;
  /// Flag indicating wether the intersection is blocked by a vehicle.
  bool _is_blocked;
  /// Bounding box that defines the intersection area.
  cv::Rect _bounding_box;
};

#endif
