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

  // TODO: Implement in the next PR.
  void processVehicleQueue();

private:
  /// List of all streets connected to this intersection.
  std::vector<std::shared_ptr<Street>> _streets;
  /// Bounding box that defines the intersection area.
  cv::Rect _bounding_box;
};

#endif
