#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "object.h"

void Object::generateModelMatrix() {
  modelMatrix =
          glm::translate(glm::mat4(1.0f), position)
          * glm::orientate4(rotation)
          * glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Object::returnModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc) {
    glm::mat4 mod =
            glm::translate(glm::mat4(1.0f), pos)
            * glm::orientate4(rot)
            * glm::scale(glm::mat4(1.0f), sc);
    return mod;
}
