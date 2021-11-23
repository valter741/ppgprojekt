// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>

// Size of the framebuffer
const unsigned int SIZE = 512;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel {
  // TODO: Define correct data type for r, g, b channel
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

using namespace std;

int main() {
    // Initialize a framebuffer
    auto framebuffer = new Pixel[SIZE][SIZE];

    // TODO: Open file lena.raw (this is 512x512 RAW GRB format)
    ifstream lena;
    lena.open("lena.raw");

    // TODO: Read data to framebuffer and close the file
    lena.read(reinterpret_cast<char *>(framebuffer), SIZE * SIZE * 3);
    lena.close();

  // Traverse the framebuffer
  for (unsigned int y = 0; y < SIZE; y++) {
    for (unsigned int x = 0; x < SIZE; x++) {
      // TODO: Apply pixel operation
      framebuffer[x][y].r = 0;
    }
  }

  // TODO: Open file result.raw
  std::cout << "Generating result.raw file ..." << std::endl;
  ofstream result;
  result.open("result.raw");

  // TODO: Write the framebuffer to the file and close it
  result.write(reinterpret_cast<char *>(framebuffer), SIZE * SIZE * 3);
  result.close();
  std::cout << "Done." << std::endl;
  delete[] framebuffer;
  return EXIT_SUCCESS;
}
