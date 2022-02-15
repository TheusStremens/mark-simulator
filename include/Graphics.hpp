#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <opencv2/core.hpp>
#include <vector>
#include <map>

enum Color
{
  RED,
  BLUE
};

class Graphics
{
public:
  Graphics();
  void loadBackgroundImage();
  void simulate();

private:

  std::map<Color, cv::Scalar> _color_map;
  std::vector<cv::Mat> _images;
};

#endif
