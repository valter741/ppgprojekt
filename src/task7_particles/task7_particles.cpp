// Task 7 - Dynamically generate objects in a 3D scene
//        - Implement a particle system where particles have position and speed
//        - Any object can be a generator and can add objects to the scene
//        - Create dynamic effect such as fireworks, rain etc.
//        - Encapsulate camera in a class

#include <iostream>
#include <vector>
#include <map>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

class Camera {
public:
  // TODO: Add parameters
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  /// Representaiton of
  /// \param fov - Field of view (in degrees)
  /// \param ratio - Viewport ratio (width/height)
  /// \param near - Distance of the near clipping plane
  /// \param far - Distance of the far clipping plane
  Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f) {
    // TODO: Initialize perspective projection (hint: glm::perspective)
    projectionMatrix = glm::perspective(fov, ratio, near, far);
    update();
  }

  /// Recalculate viewMatrix from position, rotation and scale
  void update() {
      viewMatrix = glm::lookAt(glm::vec3{0.0f, 20.0f, 20.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
  }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = std::list<std::unique_ptr<Renderable>>; // Type alias

class Renderable {

public:

    int corona = 0;
    int isCorona = 0;
  // Virtual destructor is needed for abstract interfaces
  virtual ~Renderable() = default;

  /// Render the object
  /// \param camera - Camera to use for rendering
  virtual void render(const Camera& camera) = 0;

  /// Update the object. Useful for specifing animation and behaviour.
  /// \param dTime - Time delta
  /// \param scene - Scene reference
  /// \return - Return true to keep object in scene
  virtual bool update(float dTime, Scene &scene) = 0;
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle
class Particle final : public Renderable {
  // Static resources shared between all particles
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;

  glm::vec3 speed{0,0,0};
  glm::vec3 color{1,1,1};
  glm::mat4 modelMatrix;
  float ttl = 0.0f;
public:
  /// Construct a new Particle
  /// \param p - Initial position
  /// \param s - Initial speed
  /// \param c - Color of particle
  Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c) {
    speed = s;
    color = c;
    modelMatrix = glm::translate(modelMatrix, p);
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
  }

  bool update(float dTime, Scene &scene) override {
      ttl += dTime;
      modelMatrix = glm::translate(modelMatrix, this->speed);
      //std::cout << dTime << " ";
      if(this->corona && !this->isCorona){
          this->corona = 0;
          for(int i = 0; i < 10; i++) {
              glm::vec3 pos = {0, 0, 0};
              glm::vec3 speed = {this->speed.x * ((rand() % 10) + 15), this->speed.y * ((rand() % 10) + 15),
                                 this->speed.z * ((rand() % 10) + 15)};
              glm::vec3 col = {0, 1, 0};
              glm::vec3 scale = {.2, .2, .2};
              auto particle = std::make_unique<Particle>(pos, speed, col);
              particle->isCorona = 1;
              particle->modelMatrix = modelMatrix;
              particle->modelMatrix = glm::scale(particle->modelMatrix, scale);
              scene.push_back(std::move(particle));
          }
      }

      if(ttl > 5.0f){
          return false;
      }else {
          return true;
      }

    // TODO: Animate position using speed and dTime.
    // - Return true to keep the object alive
    // - Returning false removes the object from the scene
    // - hint: you can add more particles to the scene here also
  }

  void render(const Camera& camera) override {
    // TODO: Render the object
    shader->use();

    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);

    shader->setUniform("OverallColor", color);
    shader->setUniform("ModelMatrix", modelMatrix);

    mesh->render();
    // - Use the shader
    // - Setup all needed shader inputs
    // - hint: use OverallColor in the color_vert_glsl shader for color
    // - Render the mesh
  }
};
// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Shader> Particle::shader;

class ParticleWindow : public ppgso::Window {
private:
  // Scene of objects
  Scene scene;

  // Create camera
  Camera camera = {120.0f, (float)width/(float)height, 1.0f, 400.0f};

  // Store keyboard state
  std::map<int, int> keys;
public:
  ParticleWindow() : Window{"task7_particles", SIZE, SIZE} {
    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  }

  void onKey(int key, int scanCode, int action, int mods) override {
    // Collect key state in a map
    keys[key] = action;
    if (keys[GLFW_KEY_SPACE]) {
        std::cout << "SPACE";
        glm::vec3 pos = {rand()%10-5,rand()%10-5,rand()%10-5};
        glm::vec3 speed = {((rand() %6)-3)/100.0f,((rand() %6)-3)/100.0f, ((rand() %6)-3)/100.0f};
        glm::vec3 col = {0.95, 0.82, 0.56};
        auto particle=std::make_unique<Particle>(pos, speed, col);
        scene.push_back(std::move(particle));
    }
    if (keys[GLFW_KEY_C]) {
          std::cout << "C";

        for(auto& object : scene) {
            object->corona = 1;
        }
      }
  }

  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();
    // Compute time delta
    float dTime = (float)glfwGetTime() - time;
    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update all objects in scene
    // Because we need to delete while iterating this is implemented using c++ iterators
    // In most languages mutating the container during iteration is undefined behaviour
    auto i = std::begin(scene);
    while (i != std::end(scene)) {
      // Update object and remove from list if needed
      auto obj = i->get();
      if (!obj->update(dTime, scene))
        i = scene.erase(i);
      else
        ++i;
    }

    // Render every object in scene
    for(auto& object : scene) {
      object->update(dTime, scene);
      object->render(camera);
    }
  }
};

int main() {
  // Create new window
  auto window = ParticleWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
