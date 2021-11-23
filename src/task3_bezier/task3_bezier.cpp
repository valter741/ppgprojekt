// Task 4 - Render the letter R using OpenGL
//        - Implement a function to generate bezier curve points
//        - Generate multiple points on each bezier curve
//        - Draw lines connecting the generated points using OpenGL

#include <iostream>
#include <vector>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


const unsigned int SIZE = 512;

class BezierWindow : public ppgso::Window {
private:

  // Control points for the bezier curves
  // First curve is 4 control points
  // Rest of the curves are 3 control points, each reusing the last curve end
  // Defines the letter "R"
  std::vector<glm::vec2> controlPoints = {
      { 0,  -1},
      { 0, -.3},
      { 0,  .3},
      { 0,   1},
      {.3,   1},
      {.5,   1},
      {.5,  .5},
      {.5,   0},
      {.3,   0},
      { 0,   0},
      {.3, -.3},
      {.5, -.5},
      {.5,  -1},
  };

  /*std::vector<glm::vec2> controlPoints = {
          { -1,  1},
          { 0, 1},
          { 0,  0},
          { -1,   0},
  };*/

  // This will hold the bezier curve geometry once we generate it
  std::vector<glm::vec3> points;

  // GLSL Program to use for rendering
  ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

  // These numbers are used to pass buffer data to OpenGL
  GLuint vao = 0, vbo = 0;

  glm::vec2 diffc(const glm::vec2 &p0, const glm::vec2 &p1, const float t){
      glm::vec2 newPoint = {p0[0] + ((p1[0] - p0[0]) * t), p0[1] + ((p1[1] - p0[1]) * t)};
      return newPoint;
  }

  // Compute points for Bezier curve using 4 control points
  glm::vec2 bezierPoint(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3, const float t) {
    glm::vec2 pa = diffc(p0, p1, t);
    glm::vec2 pb = diffc(p1, p2, t);
    glm::vec2 pc = diffc(p2, p3, t);

    glm::vec2 pm = diffc(pa, pb, t);
    glm::vec2 pn = diffc(pb, pc, t);

    glm::vec2 p = diffc(pm, pn, t);

    //float x = ((1-t)*(1-t)*(1-t) * p0[0]) + (3*(1-t)*(1-t)*t*p1[0]) + (3*(1-t)*t*t*p2[0] + t*t*t*p3[0]);
    //std::cout << p0[0] << " " << p0[1] << "\n";
    //float y = (pow((1-t), 3) * p0[1]) + (3*pow((1-t), 2)*t*p1[1]) + (3*(1-t)*t*t*p2[1] + t*t*t*p3[1]);

    //glm::vec2 p = {x, y};

    std::cout << p[0] << " " << p[1] << " " << t << "\n";
    return p;
  }

  // Compute points for a sequence of Bezier curves defined by a vector of control points
  // Each bezier curve will reuse the end point of the previous curve
  // count - Number of points to generate on each curve
  void bezierShape(int count) {
    for(int i = 1; i < (int) controlPoints.size(); i+=3) {
      float t = 0;
      for (int j = 0; j <= count; j++) {
        glm::vec2 point = bezierPoint(controlPoints[i-1], controlPoints[i], controlPoints[i+1], controlPoints[i+2], t);
        t += 0.1;
        points.emplace_back(point, 0);
      }
    }
  }

public:
  BezierWindow() : Window{"task3_bezier", SIZE, SIZE} {
    // Generate Bezier curve points


    bezierShape(10);

      /*points.push_back(glm::vec3 {0, 0, 0});
      points.push_back(glm::vec3 {0, 1, 0});
      points.push_back(glm::vec3 {1, 1, 0});
      points.push_back(glm::vec3 {1, 0, 0});*/


      std::cout << points.size();

    // Generate a vertex array object
    // This keeps track of what attributes are associated with buffers
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // TODO: Pass the control points to the GPU
      glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof (glm::vec3), points.data(), GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, ???, ???, GL_STATIC_DRAW);

    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
    auto position_attrib = program.getAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_attrib);

    // Set model matrix to identity
    program.setUniform("ModelMatrix", glm::mat4{});
    program.setUniform("ViewMatrix", glm::mat4{});
    program.setUniform("ProjectionMatrix", glm::mat4{});

    // Set the color uniform
    program.setUniform("OverallColor", glm::vec3{1.0f,1.0f,1.0f});
  }

  ~BezierWindow() final {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  void onIdle() final {
    // Set gray background
    glClearColor(0.5f,0.5f,0.5f,0);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw shape
    glBindVertexArray(vao);

    // TODO: Define the correct render mode
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    //glDrawArrays(??, 0, ??);
  }
};

int main() {
  // Create our window
  BezierWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
