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

      auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 200.0f);


    if(scene.scenar == 1){

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

    }else if (scene.scenar == 2){
        camera->position = {0,0,-7};
        camera->back = {0,0,-1};
        scene.camera = move(camera);

        auto skybox = std::make_unique<Skybox>();
        skybox->texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("underwater.bmp"));
        scene.objects.push_back(move(skybox));

        auto koryto = std::make_unique<Koryto2>();
        scene.objects.push_back(move(koryto));

        for(int i = 0; i < 8; i++){
            auto fish = std::make_unique<Fish>();
            scene.objects.push_back(move(fish));
        }

        for(int i = 0; i < 3; i++){
            auto bubbleGenerator = std::make_unique<BubbleGenerator>(glm::vec3(glm::linearRand(-10.0f, 10.0f),-5,10));
            scene.objects.push_back(move(bubbleGenerator));
        }

        auto bobor = std::make_unique<Bobor>();
        bobor->position = {0,10,0};
        bobor->rotation = {0,0,glm::radians(90.0f)};
        bobor->scale *= 0.5;
        scene.objects.push_back(move(bobor));

    }else if (scene.scenar == 3){
        camera->position = {28, 8, -2};
        camera->back = {5.6, 0.25, -1};
        scene.camera = move(camera);

        auto skybox = std::make_unique<Skybox>();
        scene.objects.push_back(move(skybox));

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

        auto rain = std::make_unique<Rain>(glm::vec3(0,30,10));
        scene.objects.push_back(move(rain));

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
    if (key == GLFW_KEY_9) {
        scene.wind = {-0.01,0,0.04};
    }
    if (key == GLFW_KEY_0) {
        scene.wind = {0,0,0};
    }

    if (key == GLFW_KEY_O) {
        scene.isRaining = true;
        scene.lightColor2 = {0,0,1};
        scene.lightColor = {0.7,0.7,0.7};
    }
    if (key == GLFW_KEY_P) {
        scene.isRaining = false;
        scene.lightColor2 = {0,0,0};
        scene.lightColor = {1,1,1};
    }

    if (key == GLFW_KEY_K) {
        scene.lightColor = {2,2,1.5};
    }
    if (key == GLFW_KEY_L) {
        scene.lightColor = {1,1,1};
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        scene.scenar = 1;
        scene.animationstep = 0;
        initScene();
    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        scene.scenar = 2;
        scene.animationstep = 0;
        scene.lightDirection2 = {1,0,-1};
        scene.lightColor2 = {1,0,0};
        initScene();
    }
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        scene.lightDirection = {1,1,-1};
        scene.lightDirection2 = {1,1,-1};
        scene.lightColor2 = {0,0,0};
        scene.scenar = 3;
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

    if(scene.camera->animstep == 0 && scene.camera->timepassed > 1 && scene.scenar == 1){
        scene.camera->animate(scene.camera->posKeyFrame[0], scene.camera->posKeyFrame[1], scene.camera->backKeyFrame[0], scene.camera->backKeyFrame[1], 10.0f, dt);
    }

    if(scene.scenar == 2){
        //std::cout << scene.lightDirection2.x;
        if(scene.lightanim == 1){
            scene.lightDirection2.x -= 0.05;
            if(scene.lightDirection2.x < -5)
                scene.lightanim = -1;
        }else{
            scene.lightDirection2.x += 0.05;
            if(scene.lightDirection2.x > 5)
                scene.lightanim = 1;
        }
    }

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
