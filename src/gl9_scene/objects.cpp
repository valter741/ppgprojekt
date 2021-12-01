#include "object.h"
#include "scene.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

class Particle: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 speed;


    Particle(glm::vec3 pos, glm::vec3 sp){

        position = pos;
        rotation = glm::vec3 (0, 0,0);
        speed = sp;
        scale *= 0.3f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mywotah.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

        timer = 0;


    }

    bool update(Scene &scene, float dt) override{
        this->timer += dt;
        float roundtime = std::round(timer*10);

        position = position+(speed*dt);

        generateModelMatrix();
        if(timer > 2.5f){
            return false;
        }
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        //shader->setUniform("LightDirection", scene.lightDirection);

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

    glm::vec3 posFrames[4] = {
            {-15, 2.5, 15},
            {-11, 2.5, 15},
            {-7, 5, 15},
            {-7, -4, 15}
    };

    glm::vec3 rotFrames[4] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, glm::radians(45.0f), 0},
            {0, -4, glm::radians(-90.0f)}
    };

    float inter = 0.0f;

    int animstage = 0;


    Bobor(){

        position = glm::vec3(-15,2.5,15);
        rotation = glm::vec3 (0, 0,0);
        scale *= 0.2;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bobor.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("bobor2.obj");

        timer = 0;

    }

    void animate(int p1, int p2, float length, float dt){
        inter += 1.0f*(dt/length);

        if(inter >= 1){
            inter = 0;
            position = posFrames[p2];
            animstage++;
            return;
        }
        rotation = glm::lerp(rotFrames[p1], rotFrames[p2], {inter,inter,inter});
        position = glm::lerp(posFrames[p1], posFrames[p2], {inter,inter,inter});
    }

    bool update(Scene &scene, float dt) override{
        this->timer += dt;
        //float roundtime = std::round(timer*10);

        //std::cout << roundtime << " ";

        if(scene.scenar == 1){
            if(timer > 10 && animstage == 0){
                animate(0, 1, 1.5f, dt);
            }else if(animstage == 1){
                animate(1, 2, 1.5f, dt);
            }else if(animstage == 2){
                animate(2, 3, 1.5f, dt);
            }
        }

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

class Koryto1: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Koryto1(){

        position = glm::vec3(0,0,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.5f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("grass.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("koryto1.obj");

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

class Koryto2: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Koryto2(){

        position = glm::vec3(0,-5,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.0f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("ground2.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("koryto2.obj");

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

class Voda1: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec2 textureOffset;

    bool splash = false;

    Voda1(){

        position = glm::vec3(0,-0.3,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 2.5f;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mywotah.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("vodaplane1.obj");

    }

    bool update(Scene &scene, float dt) override{

        textureOffset.y -= dt/15;

        auto start = scene.objects.begin();
        std::advance(start, 1);
        Object* bobor = start->get();
        if(bobor->position.y < this->position.y && !splash){
            //position = position = glm::vec3(0,5,0);
            splash = true;
            for(int i = 0; i < 10; i++){
                auto particle = std::make_unique<Particle>(bobor->position, glm::vec3(glm::linearRand(-2.0f, 2.0f),4,glm::linearRand(-2.0f, 2.0f)));
                scene.objects.push_back(move(particle));
            }
        }


        generateModelMatrix();
        return true;
    }


    void render(Scene &scene) override {
        shader->use();

        // Set up light
        shader->setUniform("LightDirection", scene.lightDirection);

        shader->setUniform("TextureOffset", textureOffset);

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

    bool rotLock = false;

    glm::vec3 positions[10] = {
            {-20,2.5,20},
            {-22,2.5,12},
            {-15,2.5,10},
            {-10,2.5,-5},
            {-16,2.5,0},
            {-20,2.5,-7},
            {-16,2.5,-14},
            {-11,2.5,-19},
            {-22,2.5,-21}
    };

    Tree(){

        position = glm::vec3(-20,2.5,20);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),0);
        scale *= 0.015;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("tree.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("treerotated.obj");

    }

    bool update(Scene &scene, float dt) override{

        if(this->rotation.x < 0.15 && !this->rotLock){
            this->rotation.x += dt/7;
        }else {
            this->rotLock = true;
        }

        if(this->rotation.x > -0.15 && this->rotLock){
            this->rotation.x -= dt/7;
        }else {
            this->rotLock = false;
        }

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

class Log: public Object{


public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

    Log(){

        position = glm::vec3(-8,3.5,20);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(180.0f),0);
        scale.x *= 0.28;
        scale.y *= 0.14;
        scale.z *= 0.14;
        if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("log.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("log.obj");

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

        position = glm::vec3(0,0,0);
        rotation = glm::vec3 (glm::radians(0.0f), glm::radians(0.0f),glm::radians(0.0f));
        scale *= 25;
        if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
        if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("skyy.bmp"));
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("boxx.obj");

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

