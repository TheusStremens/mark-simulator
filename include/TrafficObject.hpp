#ifndef TRAFFICOBJECT_H
#define TRAFFICOBJECT_H

#include <vector>
#include <thread>
#include <mutex>

enum ObjectType
{
  noObject,
  objectVehicle,
  objectIntersection,
  objectStreet,
};

/**
 * @brief Generic object used as a base/interface class.
 *
 */
class TrafficObject
{
public:
  /// Constructor
  TrafficObject();
  /// Desctructor.
  ~TrafficObject();

  // * Small get/set methods that are self explanatory * //
  int getID() { return _id; };
  void setPosition(double x, double y);
  void getPosition(double &x, double &y);
  ObjectType getType() { return _type; };

  // Abstract method that defines what the class will do in the simulation.
  virtual void simulate() {};

protected:
  /// Identifies the class type.
  ObjectType _type;
  /// Every traffic object has its own unique id.
  int _id;
  /// Object position in pixels.
  double _pos_x, _pos_y;
  /// Holds all _threads that have been launched within this object.
  std::vector<std::thread> _threads;
  /// Mutex shared by all traffic objects for protecting cout.
  static std::mutex _mtx;

private:
  /// Global variable for counting object ids.
  static int _id_counter;
};

#endif
