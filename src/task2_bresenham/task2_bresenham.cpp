// Task 3 - Implement Bresenham drawing alg.
//        - Draw a star using lines
//        - Make use of std::vector to define the shape
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include <ppgso/ppgso.h>

// Size of the framebuffer
const unsigned int SIZE = 512;

struct Point {
  int x,y;
};

// Bresenham drawing algorithm
void drawLine(ppgso::Image& framebuffer, Point& from, Point& to) {

    if(from.x < to.x && from.y <= to.y){
        int m_new = 2 * (to.y - from.y);
        int slope_error_new = m_new-(to.x - from.x);
        for (int x = from.x, y = from.y; x <= to.x; x++)      //DOPRAVA
        {
            //std::cout << "(" << x << "," << y << ")\n";
            framebuffer.setPixel(x,y,255,255,255);

            slope_error_new += m_new;

            while (slope_error_new >= 0)
            {
                y++;
                slope_error_new  -= 2 * (to.x - from.x);
            }
        }
    }else if (from.x >= to.x && from.y < to.y){
        int m_new = 2 * (from.x - to.x);
        int slope_error_new = m_new -(to.y - from.y);
        for (int x = from.x, y = from.y; y <= to.y; y++)    //DOLE
        {
            //std::cout << "(" << x << "," << y << ")\n";
            framebuffer.setPixel(x, y, 255, 255, 255);

            slope_error_new -= m_new;

            while (slope_error_new <= 0) {
                x--;
                slope_error_new += 2 * (to.y - from.y);
            }
        }
    }else if (from.x > to.x && from.y >= to.y){
        int m_new = 2 * (from.y - to.y);
        int slope_error_new = m_new-(from.x - to.x);
        for (int x = from.x, y = from.y; x >= to.x; x--)        //DOLAVA
        {
            //std::cout << "(" << x << "," << y << ")\n";
            framebuffer.setPixel(x,y,255,255,255);

            slope_error_new += m_new;

            while (slope_error_new >= 0)
            {
                y--;
                slope_error_new  -= 2 * (from.x - to.x);
            }
        }
    }else if (from.x <= to.x && from.y > to.y){
        int m_new = 2 * (to.x - from.x);
        int slope_error_new = m_new -(from.y - to.y);
        for (int x = from.x, y = from.y; y >= to.y; y--)    //HORE
        {
            //std::cout << "(" << x << "," << y << ")\n";
            framebuffer.setPixel(x, y, 255, 255, 255);

            slope_error_new -= m_new;

            while (slope_error_new <= 0) {
                x++;
                slope_error_new += 2 * (from.y - to.y);
            }
        }
    }
}

int main()
{
  // Use ppgso::Image as our framebuffer
  ppgso::Image framebuffer(SIZE, SIZE);

  // TODO: Generate star points
  std::vector<Point> points;
  Point newPoint;

  for(int i = 1; i <= 12; i++) {
         
  }

  // Draw lines
  for(unsigned int i = 0; i < points.size() - 1; i+=2)
    drawLine(framebuffer, points[i], points[i+1]);

  //framebuffer.setPixel(0,0,255,255,255);

  // Save the result
  std::cout << "Generating task2_bresenham.bmp file ..." << std::endl;
  ppgso::image::saveBMP(framebuffer, "task2_bresenham.bmp");

  std::cout << "Done." << std::endl;
  return EXIT_SUCCESS;
}

