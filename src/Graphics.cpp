#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Graphics.hpp"
#include "Intersection.hpp"
#include "Vehicle.hpp"
#include <iostream>

const bool kDebug = false;

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
  // Reset images.
  _images.at(1) = _images.at(0).clone();
  _images.at(2) = _images.at(0).clone();
  // Create overlay from all traffic objects.
  for (auto traffic_object : _traffic_objects)
  {
    double pos_x, pos_y;
    traffic_object->getPosition(pos_x, pos_y);

    if (traffic_object->getType() == ObjectType::objectIntersection)
    {
      // Draw a cross in the center of the intersection.
      cv::drawMarker(_images.at(1), cv::Point2d(pos_x, pos_y), cv::Scalar(255, 255, 0), cv::MARKER_TILTED_CROSS, 25);
    }
    else if (traffic_object->getType() == ObjectType::objectVehicle)
    {
      cv::RNG rng(traffic_object->getID());
      int b = rng.uniform(0, 255);
      int g = rng.uniform(0, 255);
      int r = sqrt(255 * 255 - g * g - b * b); // Ensure that length of color vector is always 255.
      cv::Scalar vehicle_color = cv::Scalar(b, g, r);
      cv::circle(_images.at(1), cv::Point2d(pos_x, pos_y), 10, vehicle_color, -1);
      if (kDebug)
      {
        // Dynamic cast to Vehicle class to get the current destination and draw a
        // rectangle on it.
        auto vehicle = dynamic_cast<Vehicle*>(traffic_object.get());
        cv::rectangle(_images.at(1), vehicle->getCurrentDestination()->getBoundingBox(), vehicle_color, 3);
      }
    }
  }

  float opacity = 0.85;
  cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

  // Display background and overlay image.
  cv::imshow("Mark Simulator", _images.at(2));
  cv::waitKey(1);
}
