#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Graphics.hpp"
#include "Intersection.hpp"
#include <iostream>

Graphics::Graphics()
{
  _color_map[Color::RED] = cv::Scalar(0, 0, 255);
  _color_map[Color::BLUE] = cv::Scalar(255, 0, 0);
}

void Graphics::loadBackgroundImage()
{
  cv::Mat background = cv::imread("../data/RoadMap.png");
  _images.push_back(background);
  _images.push_back(background.clone());
  _images.push_back(background.clone());
}

void Graphics::simulate()
{
  while (true) // ESC
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    drawTrafficObjects();
  }
}

void Graphics::drawTrafficObjects()
{
  // reset images
  _images.at(1) = _images.at(0).clone();
  _images.at(2) = _images.at(0).clone();
  // create overlay from all traffic objects
  for (auto it : _traffic_objects)
  {
    double posx, posy;
    it->getPosition(posx, posy);

    if (it->getType() == ObjectType::objectIntersection)
    {
      cv::drawMarker(_images.at(1), cv::Point2d(posx, posy), cv::Scalar(255, 255, 0), cv::MARKER_TILTED_CROSS, 25);
    }
    else if (it->getType() == ObjectType::objectVehicle)
    {
      cv::RNG rng(it->getID());
      int b = rng.uniform(0, 255);
      int g = rng.uniform(0, 255);
      int r = sqrt(255 * 255 - g * g - b * b); // ensure that length of color vector is always 255
      cv::Scalar vehicleColor = cv::Scalar(b, g, r);
      // Check if the position is outside the image limits. In this case, just put the
      // object in the "other side".
      if (posx < 0)
        posx = _images.at(0).cols + posx;
      if (posy < 0)
        posy = _images.at(0).rows + posy;
      if (posx > _images.at(0).cols)
        posx = posx - _images.at(0).cols;
      if (posy > _images.at(0).rows)
        posy = posy - _images.at(0).rows;
      cv::circle(_images.at(1), cv::Point2d(posx, posy), 10, vehicleColor, -1);
    }
  }


  float opacity = 0.85;
  cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

  // display background and overlay image
  cv::imshow("Mark Simulator", _images.at(2));
  cv::waitKey(1);
}