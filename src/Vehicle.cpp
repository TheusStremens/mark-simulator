#include <iostream>
#include <random>
#include "Street.hpp"
#include "Intersection.hpp"
#include "Vehicle.hpp"

Vehicle::Vehicle()
{
  _current_street = nullptr;
  _type = ObjectType::objectVehicle;
  _speed = 0.25;
}

void Vehicle::simulate()
{
  // Launch drive function in a thread.
  threads.emplace_back(std::thread(&Vehicle::drive, this));
}

std::shared_ptr<Intersection>
Vehicle::getPreviousIntersection()
{
  if (_current_destination->getID() == _current_street->getIntersectionA()->getID())
    return _current_street->getIntersectionB();
  return _current_street->getIntersectionA();
}

void
Vehicle::chooseStartingPosition()
{
  std::shared_ptr<Intersection> previous_intersection = getPreviousIntersection();
  double current_x, current_y, next_x, next_y;
  previous_intersection->getPosition(current_x, current_y);
  _current_destination->getPosition(next_x, next_y);
  // Loop streets are very hard to deal with it, so, simplify this case.
  if (_current_street->isLoopStreet())
  {
    _posX = current_x;
    _posY = current_y;
    return;
  }
  // Otherwise choose the position randomly along the street.
  std::random_device rd;
  std::mt19937 eng(rd());
  if (_current_street->getOrientation() == StreetOrientation::horizontal)
  {
    // For uniform_int_distribution(a, b) we need a < b.
    if (current_x > next_x)
      std::swap(current_x, next_x);
    std::uniform_int_distribution<> distr(current_x, next_x);
    _posX = distr(eng);
    _posY = current_y;
  }
  if (_current_street->getOrientation() == StreetOrientation::vertical)
  {
    // For uniform_int_distribution(a, b) we need a < b.
    if (current_y > next_y)
      std::swap(current_y, next_y);
    std::uniform_int_distribution<> distr(current_y, next_y);
    _posY = distr(eng);
    _posX = current_x;
  }
  // Apply the lane offsets.
  _posX += _current_lane->getHorizontalStreetOffset();
  _posY += _current_lane->getVerticalStreetOffset();
}

void Vehicle::pickLane()
{
  std::shared_ptr<Intersection> previous_intersection = getPreviousIntersection();;
  double current_x, current_y, next_x, next_y;
  previous_intersection->getPosition(current_x, current_y);
  _current_destination->getPosition(next_x, next_y);

  Direction new_direction;
  // Loop street inverts the direction.
  if (_current_street->getOrientation() == StreetOrientation::horizontal)
  {
    if (next_x < current_x)
      new_direction = !_current_street->isLoopStreet() ? Direction::left : Direction::right;
    if (next_x > current_x)
      new_direction = !_current_street->isLoopStreet() ? Direction::right : Direction::left;
  }
  else if (_current_street->getOrientation() == StreetOrientation::vertical)
  {
    if (next_y < current_y)
      new_direction = !_current_street->isLoopStreet() ? Direction::up : Direction::down;
    if (next_y > current_y)
      new_direction = !_current_street->isLoopStreet() ? Direction::down : Direction::up;
  }
  std::vector<std::shared_ptr<Lane>> possible_lanes = _current_street->getLanesByDirection(new_direction);
  uint num_possible_lanes = possible_lanes.size();
  if (num_possible_lanes == 0)
    throw std::runtime_error("Vehicle could not pick a lane, check the lane/street configuration");
  if (num_possible_lanes == 1)
    _current_lane = possible_lanes[0]; // Only option
  else // Choose randomly.
  {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, num_possible_lanes - 1);
    _current_lane = possible_lanes.at(distr(eng));
  }
}

void Vehicle::drive()
{
  // Print id of the current thread.
  std::unique_lock<std::mutex> lck(_mtx);
  std::cout << "Vehicle #" << _id << "::drive: thread id = " << std::this_thread::get_id() << std::endl;
  lck.unlock();

  // Initialize the stop watch.
  std::chrono::time_point<std::chrono::system_clock> last_update;
  last_update = std::chrono::system_clock::now();

  while (true)
  {
    // Sleep at every iteration to reduce CPU usage.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Compute time difference to stop watch.
    long time_since_last_update = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_update).count();
    if (time_since_last_update >= _frequency)
    {
      double target_x, target_y;
      _current_destination->getPosition(target_x, target_y);
      // The target position is the intersection position plus the lane offsets.
      target_x += _current_lane->getHorizontalStreetOffset();
      target_y += _current_lane->getVerticalStreetOffset();

      if (_current_street->getOrientation() == StreetOrientation::vertical)
      {
        // If it is not aligned horizontally.
        if (target_x != _posX)
          _direction = (target_x > _posX) ? Direction::right : Direction::left;
        else
          _direction = _current_lane->getDirection(); // Just follow the lane direction.
      }
      if (_current_street->getOrientation() == StreetOrientation::horizontal)
      {
        // If it is not aligned vertically.
        if (target_y != _posY)
          _direction = (target_y > _posY) ? Direction::down : Direction::up;
        else
          _direction = _current_lane->getDirection(); // Just follow the lane direction.
      }

      // If the vehicle entered in the intersection area.
      if (_current_destination->isInside(_posX, _posY))
      {
        // Choose next street and destination.
        std::vector<std::shared_ptr<Street>> street_options = _current_destination->queryStreets(_current_street);
        std::shared_ptr<Street> next_street;

        // Pick one street at random and query intersection to enter this street.
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<> distr(0, street_options.size() - 1);
        next_street = street_options.at(distr(eng));

        // Pick the one intersection at which the vehicle is currently not.
        std::shared_ptr<Intersection> next_intersection = next_street->getIntersectionA()->getID() == _current_destination->getID() ? next_street->getIntersectionB() : next_street->getIntersectionA();

        // send signal to intersection that vehicle has left the intersection
        // _current_destination->vehicleHasLeft(get_shared_this());

        // Assign new street and destination.
        this->setCurrentDestination(next_intersection);
        this->setCurrentStreet(next_street);
        // We can choose the next lane.
        pickLane();
      }

      // Update position.
      switch (_direction)
      {
        case Direction::up:
          _posY -= _speed;
          break;
        case Direction::down:
          _posY += _speed;
          break;
        case Direction::left:
          _posX -= _speed;
          break;
        case Direction::right:
          _posX += _speed;
          break;
      }

      // Loop position in the image.
      if (_posX < 0)
        _posX =  999;
      if (_posX > 999)
        _posX =  0;
      if (_posY < 0)
        _posY =  999;
      if (_posY > 999)
        _posY =  0;


      // Reset stop watch for next cycle.
      last_update = std::chrono::system_clock::now();
    }
  } // EOF simulation loop.
}
