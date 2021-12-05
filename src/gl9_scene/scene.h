#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"


/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
  public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;

    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{0, 1.0f, -1.0f};

    glm::vec3 lightColor{1, 1, 1};

    glm::mat3 filtr = {
            {0,0,0},
            {0,1,0},
            {0,0,0}
    };

    glm::vec3 lightDirection2{0, 1.0f, -1.0f};

    glm::vec3 lightColor2{0, 0, 0};

    // Store cursor state
    struct {
      double x, y;
      bool left, right;
    } cursor;

    int scenar = 1;

    int animationstep = -1;

    int lightanim = 1;

    glm::vec3 gravity = {0, -0.2, 0};
    glm::vec3 wind = {0, 0, 0};

    bool isRaining = false;

    bool zaplava = false;
};

#endif // _PPGSO_SCENE_H
