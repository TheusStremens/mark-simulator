#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Graphics.hpp"
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
}

void Graphics::simulate()
{
  char c = ' ';
  while (c != 27) // ESC
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    cv::imshow("Mark Simulator", _images.at(0));
    c = cv::waitKey(1);
  }
}
