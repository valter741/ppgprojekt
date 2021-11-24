#include "object.h"
#include "scene.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>




class Cat: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Cat(){

        position = glm::vec3(0,0,0);
        rotation = glm::vec3 (glm::radians(-90.0f), glm::radians(180.0f),0);
        scale *= 0.2;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("cat.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cat.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Bobor: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Bobor(){

        position = glm::vec3(-15,0,0);
        rotation = glm::vec3 (glm::radians(-90.0f), 0,0);
        //scale *= 0.2;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bobor.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bobor.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};


class Tree: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Tree(){

        position = glm::vec3(10,5,0);
        rotation = glm::vec3 (glm::radians(-90.0f), glm::radians(90.0f),0);
        scale *= 0.02;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("tree.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("tree.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);

        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();
    }
};

class Skybox: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Skybox(){

        position = glm::vec3(0,0,-10);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(0.0f));
        scale *= 50;
        if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sky.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Crate1.obj");

    }

    bool update(Scene &scene, float dt) override{
        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        //glDepthFunc(GL_ALWAYS);
        glFrontFace(GL_CW);
        // Set up light
        //shader->setUniform("LightDirection", scene.lightDirection);
        // use camera
        shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("Texture", *texture);
        mesh->render();

        glFrontFace(GL_CCW);
        //glDepthFunc(GL_LEQUAL);

    }
};