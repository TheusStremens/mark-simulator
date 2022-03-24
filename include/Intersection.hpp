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

/**
 * @brief Sizes of the Rect that defines the intersection in the image.
 *
 */
const cv::Size kBigSquare = cv::Size(80, 80);
const cv::Size kSmallSquare = cv::Size(40, 40);
const cv::Size kHorizontalWideSquare = cv::Size(80, 40);
const cv::Size kVerticalWideSquare = cv::Size(40, 80);

/**
 * @brief Auxiliary class to queue and dequeue waiting vehicles in a thread-safe manner
 *
 */
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

/**
 * @brief Class that simulates a intersection.
 *
 */
class Intersection : public TrafficObject
{
public:
  /// Constructor
  Intersection();

  // * Inline small methods that are self explanatory * //
  inline void addStreet(std::shared_ptr<Street> street) { _streets.push_back(street); }
  inline cv::Rect getBoundingBox() { return _bounding_box; }
  inline void setIsBlocked(bool isBlocked) { _is_blocked = isBlocked; };
  inline void vehicleHasLeft(std::shared_ptr<Vehicle> vehicle) { this->setIsBlocked(false); };

  /**
   * @brief Add vehicle in the queue of waiting vehicles to have permission to entry.
   *
   * @param vehicle
   */
  void addVehicleToQueue(std::shared_ptr<Vehicle> vehicle);

  /**
   * @brief Return a list of all streets connected to this intersection except the
   * incoming street.
   *
   * @param incoming
   * @return std::vector<std::shared_ptr<Street>>
   */
  std::vector<std::shared_ptr<Street>> queryStreets(std::shared_ptr<Street> incoming);

  /**
   * @brief Launch the intersection thread that controls what the intersection should
   * do.
   *
   */
  void simulate();

  /**
   * @brief Given a bounding box size, set the internal member using the size and the
   * position.
   *
   * @param bb_size
   */
  void setBoundingBoxSize(const cv::Size &bb_size);

  /**
   * @brief Check if the point (pos_x, pos_y) is inside the bounding box.
   *
   * @param pos_x
   * @param pos_y
   * @return true
   * @return false
   */
  bool isInside(const int &pos_x, const int &pos_y);

  /**
   * @brief Check if the point (pos_x, pos_y) is inside the bounding box counting the
   * vehicle size.
   *
   * @param pos_x
   * @param pos_y
   * @return true
   * @return false
   */
  bool isCompletelyInside(const int &pos_x, const int &pos_y);

  /**
   * @brief Check the queue and block if it has a vehicle waiting. Grant access to that
   * vehicle.
   *
   */
  void processVehicleQueue();

private:
  /// List of all streets connected to this intersection.
  std::vector<std::shared_ptr<Street>> _streets;
  /// List of all vehicles and their associated promises waiting to enter the intersection.
  WaitingVehicles _waiting_vehicles;
  /// Flag indicating wether the intersection is blocked by a vehicle.
  bool _is_blocked;
  /// Bounding box that defines the intersection area.
  cv::Rect _bounding_box;
};

#endif
