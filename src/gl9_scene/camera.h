#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
  glm::vec3 up{0,1,0};
  glm::vec3 position{16,24,-28};
  glm::vec3 back{0.65,0.65,-1};

  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  glm::vec3 posKeyFrame[8] = {
          {16,24,-28},
          {6, 10, 5},
          {0,0,0},
          {28,8,-2},
          {18,8,10},
          {2,8,20},
          {17,8,20},
          {16,24,-28}
  };
  glm::vec3 backKeyFrame[8] = {
          {0.65, 0.65, -1},
          {1.5,  0.7,  -1},
          {0,0,0},
          {5.6,0.25,-1},
          {1.39,0.3,1},
          {1.35,0.3,1},
          {0.5,0.3,1},
          {0.65, 0.65, -1}
  };

  float timeFrame[8] = {10.0f, 0, 0, 15.0f, 15.0f, 15.0f, 15.0f, 0};

  float inter = 0.0f;
  float timepassed = 0.0f;
  int animstep = 0;



  /*!
   * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
   * @param fow - Field of view in degrees
   * @param ratio - Viewport screen ratio (usually width/height of the render window)
   * @param near - Distance to the near frustum plane
   * @param far - Distance to the far frustum plane
   */
  Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

  void animate(glm::vec3 pkey1, glm::vec3 pkey2, glm::vec3 vkey1, glm::vec3 vkey2, float length, float dt);

  /*!
   * Update Camera viewMatrix based on up, position and back vectors
   */
  void update(float dt);

  /*!
   * Get direction vector in world coordinates through camera projection plane
   * @param u - camera projection plane horizontal coordinate [-1,1]
   * @param v - camera projection plane vertical coordinate [-1,1]
   * @return Normalized vector from camera position to position on the camera projection plane
   */
  glm::vec3 cast(double u, double v);
};

