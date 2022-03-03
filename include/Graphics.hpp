#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <opencv2/core.hpp>
#include <vector>
#include <map>
#include "TrafficObject.hpp"

enum Color
{
  RED,
  BLUE
};

/**
 * @brief Class that renders the simulation.
 *
 */
class Graphics
{
public:
  /// Constructor.
  Graphics();
  /// Load the display image.
  void loadBackgroundImage();
  /// Infinit loop that draw all the objects.
  void simulate();

  inline void setTrafficObjects(std::vector<std::shared_ptr<TrafficObject>> &traffic_objects)
  {
    _traffic_objects = traffic_objects;
  };

private:

  void drawTrafficObjects();

  std::vector<std::shared_ptr<TrafficObject>> _traffic_objects;
  std::map<Color, cv::Scalar> _color_map;
  std::vector<cv::Mat> _images;
};

#endif
