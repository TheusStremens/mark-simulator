#include <iostream>
#include <thread>
#include "Graphics.hpp"

int main(int argc, char *argv[])
{
  Graphics graphics;
  graphics.loadBackgroundImage();
  graphics.simulate();
  return 0;
}
