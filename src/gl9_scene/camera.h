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

  glm::vec3 posKeyFrame[2] = {
          {16,24,-28},
          {6, 10, 5}
  };
  glm::vec3 backKeyFrame[2] = {
          {0.65,0.65,-1},
          {1.5, 0.7, -1}
  };;

  float inter = 0.0f;
  float timepassed = 0.0f;
  bool anim = false;



  /*!
   * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
   * @param fow - Field of view in degrees
   * @param ratio - Viewport screen ratio (usually width/height of the render window)
   * @param near - Distance to the near frustum plane
   * @param far - Distance to the far frustum plane
   */
  Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

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

