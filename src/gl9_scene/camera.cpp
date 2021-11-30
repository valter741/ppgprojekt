#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
  float fowInRad = (ppgso::PI/180.0f) * fow;

  projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
  viewMatrix = lookAt(position, position-back, up);
}

void Camera::animate(glm::vec3 pkey1, glm::vec3 pkey2, glm::vec3 vkey1, glm::vec3 vkey2, float length, float dt){
    if(!anim){
        anim = true;
        inter = 1.0f*(dt/length);
    }
    if(inter > 1){
        inter = 1;
        anim = false;
    }
    position = glm::lerp(pkey1, pkey2, {inter,inter,inter});
    back = glm::lerp(vkey1, vkey2, {inter,inter,inter});
    inter += 1.0f*(dt/length);
}

void Camera::update(float dt) {

    timepassed += dt;

    if(timepassed > 1 && position.x != posKeyFrame[1].x){
        animate(posKeyFrame[0], posKeyFrame[1], backKeyFrame[0], backKeyFrame[1], 3.0f, dt);
    }

    viewMatrix = lookAt(position, position-back, up);
}

glm::vec3 Camera::cast(double u, double v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - glm::vec4{position,1.0f});
  return glm::vec3{direction};
}
