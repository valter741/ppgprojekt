// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "player.h"
#include "space.h"

#include "objects.cpp"

const unsigned int SIZE = 900;


/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
  Scene scene;
  bool animate = true;
  static float time;
  //Camera camera = Camera(60.0f, 1.0f, 0.1f, 100.0f);
  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.objects.clear();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 200.0f);
    scene.camera = move(camera);

    auto skybox = std::make_unique<Skybox>();
    scene.objects.push_back(move(skybox));
    // Add space background
    //scene.objects.push_back(std::make_unique<Space>());

    auto bobor = std::make_unique<Bobor>();
    scene.objects.push_back(move(bobor));

    auto voda1 = std::make_unique<Voda1>();
    scene.objects.push_back(move(voda1));

    auto koryto1 = std::make_unique<Koryto1>();
    scene.objects.push_back(move(koryto1));

    auto log = std::make_unique<Log>();
    scene.objects.push_back(move(log));

    for(int i = 0; i < 9; i++){
        auto tree = std::make_unique<Tree>();
        tree->position = tree->positions[i];
        scene.objects.push_back(move(tree));
    }
  }

public:
  /*!
   * Construct custom game window
   */
  SceneWindow() : Window{"gl9_scene", SIZE, SIZE} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    initScene();
  }

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    if (key == GLFW_KEY_M) {
        std::cout << "pos: " << scene.camera->position.x << " " << scene.camera->position.y << " " <<scene.camera->position.z << "\n" ;
        std::cout << "back: " << scene.camera->back.x << " " << scene.camera->back.y << " " << scene.camera->back.z << "\n" ;
    }
    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }

    if (key == GLFW_KEY_A) {
      scene.camera->position.x += 1;
    }

    if (key == GLFW_KEY_D) {
      scene.camera->position.x -= 1;
    }

    if (key == GLFW_KEY_W) {
      scene.camera->position.z += 1;
    }

    if (key == GLFW_KEY_S) {
      scene.camera->position.z -= 1;
    }

    if (key == GLFW_KEY_UP) {
      scene.camera->position.y += 1;
    }
    if (key == GLFW_KEY_DOWN) {
      scene.camera->position.y -= 1;
    }
    if (key == GLFW_KEY_LEFT) {
      scene.camera->back.x -= 1;
    }
    if (key == GLFW_KEY_RIGHT) {
      scene.camera->back.x += 1;
    }
    // otocenie kamery o 180stupnov dozadu/dopredu
    if (key == GLFW_KEY_1) {
      if (scene.camera->back.z == -1)
        scene.camera->back.z = 1;
    }
    if (key == GLFW_KEY_2) {
      if (scene.camera->back.z == 1)
        scene.camera->back.z = -1;
    }

    if (key == GLFW_KEY_F1) {
        scene.scenar = 1;
        initScene();
    }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;

    if (scene.cursor.left) {

      double u = - (cursorX / width - 0.5f) * 2.0f;
      double v = - (cursorY / height - 0.5f) * 2.0f;
      scene.camera->back.x = u;
      scene.camera->back.y = v;
    }
  }

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      scene.cursor.left = action == GLFW_PRESS;

      if (scene.cursor.left) {
        // Convert pixel coordinates to Screen coordinates
        double u = (scene.cursor.x / width - 0.5f) * 2.0f;
        double v = - (scene.cursor.y / height - 0.5f) * 2.0f;

        // Get mouse pick vector in world coordinates
        auto direction = scene.camera->cast(u, v);
        auto position = scene.camera->position;

        // Get all objects in scene intersected by ray
        auto picked = scene.intersect(position, direction);

        // Go through all objects that have been picked
        for (auto &obj: picked) {
          // Pass on the click event
          obj->onClick(scene);
        }

          //scene.camera->back.x = u;
          //scene.camera->back.y = v;
      }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static float time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;
    if(dt > 0.1)
        dt = 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    scene.update(dt);
    scene.render();
  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
