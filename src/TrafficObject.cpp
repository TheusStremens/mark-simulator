#include <algorithm>
#include <iostream>
#include <chrono>
#include "TrafficObject.hpp"

// Init static variable.
int TrafficObject::_id_counter = 0;

std::mutex TrafficObject::_mtx;

void TrafficObject::setPosition(double x, double y)
{
  _pos_x = x;
  _pos_y = y;
}

void TrafficObject::getPosition(double &x, double &y)
{
  x = _pos_x;
  y = _pos_y;
}

TrafficObject::TrafficObject()
{
  _type = ObjectType::noObject;
  _id = _id_counter++;
}

TrafficObject::~TrafficObject()
{
  // Set up thread barrier before this object is destroyed
  std::for_each(_threads.begin(), _threads.end(), [](std::thread &t)
                { t.join(); });
}
