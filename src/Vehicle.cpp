#include <iostream>
#include <random>
#include "Street.hpp"
#include "Intersection.hpp"
#include "Vehicle.hpp"

Vehicle::Vehicle()
{
  _currStreet = nullptr;
  _posStreet = 0.0;
  _type = ObjectType::objectVehicle;
  _speed = 400; // m/s
}

void Vehicle::setCurrentDestination(std::shared_ptr<Intersection> destination)
{
  // update destination
  _currDestination = destination;

  // reset simulation parameters
  _posStreet = 0.0;
}

void Vehicle::simulate()
{
  // launch drive function in a thread
  threads.emplace_back(std::thread(&Vehicle::drive, this));
}

void Vehicle::pickLane()
{

  std::shared_ptr<Intersection> current_intersection;
  current_intersection = _currDestination->getID() == _currStreet->getInIntersection()->getID()
                         ? _currStreet->getOutIntersection()
                         : _currStreet->getInIntersection();
  double current_x, current_y, next_x, next_y;
  current_intersection->getPosition(current_x, current_y);
  _currDestination->getPosition(next_x, next_y);

  Direction new_direction;
  if (next_x < current_x)
    new_direction = !_currStreet->isLoopStreet() ? Direction::left : Direction::right;
  if (next_x > current_x)
    new_direction = !_currStreet->isLoopStreet() ? Direction::right : Direction::left;
  if (next_y < current_y)
    new_direction = !_currStreet->isLoopStreet() ? Direction::up : Direction::down;
  if (next_y > current_y)
    new_direction = !_currStreet->isLoopStreet() ? Direction::down : Direction::up;
  std::vector<std::shared_ptr<Lane>> possible_lanes = _currStreet->getLanesByDirection(new_direction);
  uint num_possible_lanes = possible_lanes.size();
  if (num_possible_lanes == 1)
    _current_lane = possible_lanes[0];
  else
  {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, num_possible_lanes - 1);
    _current_lane = possible_lanes.at(distr(eng));
  }
}

// virtual function which is executed in a thread
void Vehicle::drive()
{
  // print id of the current thread
  std::unique_lock<std::mutex> lck(_mtx);
  std::cout << "Vehicle #" << _id << "::drive: thread id = " << std::this_thread::get_id() << std::endl;
  lck.unlock();

  // initialize variables
  bool hasEnteredIntersection = false;
  double cycleDuration = 1; // duration of a single simulation cycle in ms
  std::chrono::time_point<std::chrono::system_clock> lastUpdate;

  // init stop watch
  lastUpdate = std::chrono::system_clock::now();
  while (true)
  {
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // compute time difference to stop watch
    long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
    if (timeSinceLastUpdate >= cycleDuration)
    {
      double target_x, target_y;
      _currDestination->getPosition(target_x, target_y);
      target_x += _current_lane->getHorizontalStreetOffset();
      target_y += _current_lane->getVerticalStreetOffset();

      if (_currStreet->getOrientation() == StreetOrientation::vertical)
      {
        // Not aligned horizontally.
        if (target_x != _posX)
          _direction = (target_x > _posX) ? Direction::right : Direction::left;
        else
          _direction = _current_lane->getDirection();
      }
      if (_currStreet->getOrientation() == StreetOrientation::horizontal)
      {
        // Not aligned vertically.
        if (target_y != _posY)
          _direction = (target_y > _posY) ? Direction::down : Direction::up;
        else
          _direction = _current_lane->getDirection();
      }

      if (_currDestination->isInside(_posX, _posY))
      {
        // choose next street and destination
        std::vector<std::shared_ptr<Street>> streetOptions = _currDestination->queryStreets(_currStreet);
        std::shared_ptr<Street> nextStreet;
        if (streetOptions.size() > 0)
        {
          // pick one street at random and query intersection to enter this street
          std::random_device rd;
          std::mt19937 eng(rd());
          std::uniform_int_distribution<> distr(0, streetOptions.size() - 1);
          nextStreet = streetOptions.at(distr(eng));
        }
        else
        {
          // this street is a dead-end, so drive back the same way
          nextStreet = _currStreet;
        }

        // pick the one intersection at which the vehicle is currently not
        std::shared_ptr<Intersection> nextIntersection = nextStreet->getInIntersection()->getID() == _currDestination->getID() ? nextStreet->getOutIntersection() : nextStreet->getInIntersection();

        // send signal to intersection that vehicle has left the intersection
        // _currDestination->vehicleHasLeft(get_shared_this());

        // assign new street and destination
        this->setCurrentDestination(nextIntersection);
        this->setCurrentStreet(nextStreet);
        pickLane();
      }

      // Update position.
      switch (_direction)
      {
        case Direction::up:
          _posY -= 0.25;
          break;
        case Direction::down:
          _posY += 0.25;
          break;
        case Direction::left:
          _posX -= 0.25;
          break;
        case Direction::right:
          _posX += 0.25;
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


      // reset stop watch for next cycle
      lastUpdate = std::chrono::system_clock::now();
    }
  } // eof simulation loop
}
