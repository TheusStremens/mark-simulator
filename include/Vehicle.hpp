#ifndef VEHICLE_H
#define VEHICLE_H

#include "TrafficObject.hpp"
#include "Lane.hpp"

// Forward declarations to avoid include cycle.
class Street;
class Intersection;

/**
 * @brief This class moves along the image based on driving behavior, moving from one
 * intersection to another, choosing the correct lanes. In case of multiples lanes it
 * choose one randomly.
 *
 */
class Vehicle : public TrafficObject, public std::enable_shared_from_this<Vehicle>
{
public:
  /// Constructor.
  Vehicle();

  /**
   * @brief Launch the drive thread which defines the behavior of the vehicle.
   *
   */
  void simulate();

  /**
   * @brief This method compares the previous intersection with the next one
   * (destination) to know what is the direction that the vehicle is moving. So, for
   * example, if the next intersection as a higher X position than the previous one, it
   * means that the vehicle should move to the right in the image. The only exception to
   * this are streets that are loops, which inverts the behavior. Since the direction is
   * known, choose one lane of the street that matches this direction. In case of more
   * than one possibility of lane, choose randomly between the available options.
   *
   */
  void pickLane();

  /**
   * @brief Define what is the starting position along the street.
   *
   */
  void chooseStartingPosition();

  // * Inline small methods that are self explanatory * //
  inline void setCurrentStreet(std::shared_ptr<Street> street) { _current_street = street; }
  inline void setCurrentDestination(std::shared_ptr<Intersection> destination) { _current_destination = destination; };
  inline std::shared_ptr<Intersection> getCurrentDestination() { return _current_destination; }

  /// Miscellaneous.
  std::shared_ptr<Vehicle> get_shared_this() { return shared_from_this(); }

private:
  /**
   * @brief Main method that controls the direction of the vehicle. First it aligns the
   * vehicle with the lane using the lane's offsets. Then move towards the lane's
   * direction. If the vehicle enters in an intersection, choose one street from that
   * intersection randomly to go and set the destination as the other intersection that
   * connects the new street.
   *
   */
  void drive();

  /**
   * @brief Based on the current street, infer what was the previous intersection.
   *
   */
  void calculatePreviousIntersection();

  /// Street on which the vehicle is currently on.
  std::shared_ptr<Street> _current_street;
  /// Destination to which the vehicle is currently driving.
  std::shared_ptr<Intersection> _current_destination;
  /// The intersection that the vehicle was before.
  std::shared_ptr<Intersection> _previous_intersection;
  /// Ego speed in pixels.
  double _speed;
  /// Lane that the vehicle is currently on.
  std::shared_ptr<Lane> _current_lane;
  /// The direction of the vehicle's movement.
  Direction _direction;
  /// The frequency in ms that the vehicle should updates its internal states.
  double _frequency{1.0};
};

#endif
